#include "PTPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Character/PTAICharacter.h"
#include "Controller/PTPlayerController.h"
#include "System/PTGameModeBase.h"
#include "System/PTGameStateBase.h"

APTPlayerCharacter::APTPlayerCharacter() : bCanAttack(true), AttackMontagePlayTime(0.f), bIsDead(false)
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(GetRootComponent());
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

void APTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		
		if (IsValid(PC))
		{
			UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
			if (IsValid(EILPS))
			{
				EILPS->AddMappingContext(IMC, 0);
			}
		}
	}
	
	if (IsValid(AttackMontage))
	{
		AttackMontagePlayTime = AttackMontage->GetPlayLength();
	}
}

void APTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (IsValid(EIC))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveInput);
		
		EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookInput);
		
		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
		
		EIC->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::AttackInput);
	}
}

void APTPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, bCanAttack);
}

void APTPlayerCharacter::MoveInput(const FInputActionValue& Value)
{
	if (!IsValid(Controller))
	{
		return;
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator ControlYawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void APTPlayerCharacter::LookInput(const FInputActionValue& Value)
{
	if (!IsValid(Controller))
	{
		return;
	}
	
	const FVector2D LookVector = Value.Get<FVector2D>();
	
	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void APTPlayerCharacter::AttackInput(const FInputActionValue& Value)
{
	if (bCanAttack && !GetCharacterMovement()->IsFalling())
	{
		ServerRPCAttack();
		
		if (!HasAuthority() && IsLocallyControlled())
		{
			PlayAttackMontage();
		}
	}
}

void APTPlayerCharacter::CheckAttackHit()
{
	if (HasAuthority())
	{
		TArray<FHitResult> OutHitResults;
		TSet<ACharacter*> DamagedCharacters;
		FCollisionQueryParams Params(NAME_None, false, this);
	
		const float AttackRange = 50.f;
		const float AttackRadius = 50.f;
	
		const FVector Forward = GetActorForwardVector();
		const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + GetActorForwardVector() * AttackRange;

		bool bIsHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(AttackRadius), Params);
	
		if (bIsHitDetected == true)
		{
			for (auto const& OutHitResult : OutHitResults)
			{
				ACharacter* DamagedCharacter = Cast<ACharacter>(OutHitResult.GetActor());
				if (IsValid(DamagedCharacter) == true)
				{
					DamagedCharacters.Add(DamagedCharacter);
				}
			}
		
			for (auto const& DamagedCharacter : DamagedCharacters)
			{
				APTPlayerCharacter* HitPlayerCharacter = Cast<APTPlayerCharacter>(DamagedCharacter);
				if (IsValid(HitPlayerCharacter))
				{
					HitPlayerCharacter->OnHitByAttack();
					continue;
				}
			
				APTAICharacter* HitAICharacter = Cast<APTAICharacter>(DamagedCharacter);
				if (IsValid(HitAICharacter))
				{
					HitAICharacter->OnHitByAttack();
				}
			}
		}
	}
}

void APTPlayerCharacter::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd,
	FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.0f);
}

void APTPlayerCharacter::ServerRPCAttack_Implementation()
{
	bCanAttack = false;

	OnRep_CanAttack();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateLambda([&]()
		{
			bCanAttack = true;
			OnRep_CanAttack();
		
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}),
		AttackMontagePlayTime + 0.5, // 공격 페널티 0.5초
		false);
	
	PlayAttackMontage();
	MulticastRPCAttack();
}

void APTPlayerCharacter::MulticastRPCAttack_Implementation()
{
	if (!HasAuthority() && !IsLocallyControlled())
	{
		PlayAttackMontage();
	}
}

void APTPlayerCharacter::OnRep_CanAttack()
{
	if (bCanAttack)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

void APTPlayerCharacter::PlayAttackMontage()
{
	if (!IsValid(AttackMontage) && !IsValid(GetMesh()))
	{
		return;
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	if (IsValid(AnimInstance))
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(AttackMontage, 1.0f);
	}
}

void APTPlayerCharacter::OnHitByAttack()
{
	APTGameStateBase* PTGS = Cast<APTGameStateBase>(GetWorld()->GetGameState());
	
	if (!IsValid(PTGS) || PTGS->MatchState != EMatchState::Playing || !HasAuthority() || bIsDead)
	{
		return;
	}
	
	APTGameModeBase* PTGM = Cast<APTGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(PTGM))
	{
		APTPlayerController* PC = Cast<APTPlayerController>(GetController());
		if (IsValid(PC))
		{
			PTGM->OnCharacterDead(PC);
		}
	}
	
	bIsDead = true;
		
	// 모든 클라이언트에 사망 처리
	MulticastRPCDie();
	
	// 바로 Destroy
	Destroy();
}

void APTPlayerCharacter::MulticastRPCDie_Implementation()
{
	// 이동 비활성화
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	// 콜리전 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// 입력 비활성화
	if (IsLocallyControlled())
	{
		DisableInput(Cast<APlayerController>(GetController()));
	}
}
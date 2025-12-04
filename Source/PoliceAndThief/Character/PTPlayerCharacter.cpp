#include "PTPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APTPlayerCharacter::APTPlayerCharacter()
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
}

void APTPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	
}

void APTPlayerCharacter::ServerRPCAttack_Implementation()
{
}

void APTPlayerCharacter::MulticastRPCAttack_Implementation()
{
}

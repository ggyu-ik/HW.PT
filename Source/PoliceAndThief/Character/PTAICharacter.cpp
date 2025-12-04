#include "Character/PTAICharacter.h"

#include "Controller/PTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"

APTAICharacter::APTAICharacter() : bIsDead(false)
{
	PrimaryActorTick.bCanEverTick = false;
	
	AIControllerClass = APTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetIsReplicated(true);
		GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = false;
		GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	}
	
	// Mesh가 클라이언트에서도 애니메이션을 업데이트하도록 설정
	if (GetMesh())
	{
		GetMesh()->SetComponentTickEnabled(true);
		GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
		GetMesh()->bOwnerNoSee = false;
		GetMesh()->bOnlyOwnerSee = false;
		GetMesh()->SetTickGroup(ETickingGroup::TG_PrePhysics);
	}
}

void APTAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;
		
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);
	}
}

void APTAICharacter::OnHitByAttack()
{
	if (!HasAuthority() || bIsDead)
	{
		return;
	}
	
	bIsDead = true;
	
	// 모든 클라이언트에 사망 처리
	MulticastRPCDie();
	
	// 바로 Destroy
	Destroy();
}

void APTAICharacter::MulticastRPCDie_Implementation()
{
	// 이동 비활성화
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	// 콜리전 비활성화
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	// AI 컨트롤러 비활성화
	AController* AIController = GetController();
	if (IsValid(AIController))
	{
		AIController->UnPossess();
	}
}
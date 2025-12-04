#include "Character/PTAICharacter.h"

#include "Controller/PTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

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
	
	MulticastRPCDie();
	
	Destroy();
}

void APTAICharacter::MulticastRPCDie_Implementation()
{
	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AController* AIController = GetController();
	if (IsValid(AIController))
	{
		AIController->UnPossess();
	}
	
	if (IsValid(Particle))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, GetActorLocation(), GetActorRotation());
	}
}
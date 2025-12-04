#include "Character/PTAICharacter.h"

#include "Controller/PTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

APTAICharacter::APTAICharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AIControllerClass = APTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	bReplicates = true;
	SetReplicateMovement(true);
	
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->SetIsReplicated(true);
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
#include "Animation/PTCharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Character/PTPlayerCharacter.h"

void UPTCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
		if (OwnerCharacter)
		{
			OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
			OwnerPlayerCharacter = Cast<APTPlayerCharacter>(OwnerCharacter);
		}
	}
	
	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacterMovementComponent))
	{
		return;
	}
	
	Velocity = OwnerCharacterMovementComponent->Velocity;
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bShouldMove = 3.f < GroundSpeed;
	bIsFalling = (OwnerCharacterMovementComponent->IsFalling());
}

void UPTCharacterAnimInstance::AnimNotify_CheckAttackHit()
{
	if (IsValid(OwnerPlayerCharacter))
	{
		OwnerPlayerCharacter->ServerRPCCheckAttackHit();
	}
}

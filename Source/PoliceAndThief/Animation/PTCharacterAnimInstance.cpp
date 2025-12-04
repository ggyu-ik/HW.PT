#include "Animation/PTCharacterAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UPTCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
		if (OwnerCharacter)
		{
			OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
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

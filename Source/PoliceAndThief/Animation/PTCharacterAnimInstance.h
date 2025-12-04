#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/PTPlayerCharacter.h"
#include "PTCharacterAnimInstance.generated.h"

class APTPlayerCharacter;
class APTPlayerCharacterMovementComponent;

UCLASS()
class POLICEANDTHIEF_API UPTCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY()
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bShouldMove : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 bIsFalling : 1;
};

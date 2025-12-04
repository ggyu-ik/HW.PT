#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PTAICharacter.generated.h"

UCLASS()
class POLICEANDTHIEF_API APTAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APTAICharacter();
	
	virtual void BeginPlay() override;
	
	void OnHitByAttack();

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCDie();
	
	bool bIsDead;
};

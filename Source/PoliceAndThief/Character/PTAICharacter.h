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
};

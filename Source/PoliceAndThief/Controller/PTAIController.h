#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PTAIController.generated.h"

class UBlackboardComponent;
UCLASS()
class POLICEANDTHIEF_API APTAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	static const float PatrolRadius;
	
	static const FName StartPatrolPositionKey;
	static const FName EndPatrolPositionKey;
	
	APTAIController();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void OnUnPossess() override;
	
	void BeginAI(APawn* InPawn);
	
	void EndAI();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};

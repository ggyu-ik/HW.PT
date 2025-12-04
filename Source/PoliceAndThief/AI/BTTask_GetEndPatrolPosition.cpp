#include "AI/BTTask_GetEndPatrolPosition.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/PTAICharacter.h"
#include "Controller/PTAIController.h"

UBTTask_GetEndPatrolPosition::UBTTask_GetEndPatrolPosition()
{
	NodeName = TEXT("GetEndPatrolPosition");
}

EBTNodeResult::Type UBTTask_GetEndPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (EBTNodeResult::Failed == Result)
	{
		return Result;
	}
	
	APTAIController* AIController = Cast<APTAIController>(OwnerComp.GetAIOwner());
	if (IsValid(AIController))
	{
		APTAICharacter* AICharacter = Cast<APTAICharacter>(AIController->GetPawn());
		
		if (IsValid(AICharacter))
		{
			UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(AIController->GetWorld());
			
			if (IsValid(NavigationSystem))
			{
				FVector StartPatrolPosition = OwnerComp.GetBlackboardComponent()->GetValueAsVector(APTAIController::StartPatrolPositionKey);
				FNavLocation EndPatrolPosition;
				if (NavigationSystem->GetRandomPointInNavigableRadius(StartPatrolPosition, AIController->PatrolRadius,EndPatrolPosition))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(APTAIController::EndPatrolPositionKey, EndPatrolPosition.Location);
					return Result = EBTNodeResult::Succeeded;
				}
			}
		}
	}
	
	return Result;
}
#include "Controller/PTAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const float APTAIController::PatrolRadius(500.f);
const FName APTAIController::StartPatrolPositionKey(TEXT("StartPatrolPosition"));
const FName APTAIController::EndPatrolPositionKey(TEXT("EndPatrolPosition"));

APTAIController::APTAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BrainComponent"));
}

void APTAIController::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn))
	{
		BeginAI(ControlledPawn);
	}
}

void APTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (IsValid(InPawn))
	{
		BeginAI(InPawn);
	}
}

void APTAIController::OnUnPossess()
{
	EndAI();
	
	Super::OnUnPossess();
}

void APTAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	EndAI();
	
	Super::EndPlay(EndPlayReason);
}

void APTAIController::BeginAI(APawn* InPawn)
{
	if (!IsValid(InPawn) || !IsValid(BehaviorTree) || !IsValid(BlackboardDataAsset))
	{
		return;
	}
	
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent) && BehaviorTreeComponent->IsRunning())
	{
		return;
	}
	
	UBlackboardComponent* BlackboardComponent = Cast<UBlackboardComponent>(Blackboard);
	if (IsValid(BlackboardComponent))
	{
		if (UseBlackboard(BlackboardDataAsset, BlackboardComponent))
		{
			bool bRunSucceeded = RunBehaviorTree(BehaviorTree);
			if (bRunSucceeded)
			{
				BlackboardComponent->SetValueAsVector(StartPatrolPositionKey, InPawn->GetActorLocation());
			}
		}
	}
}

void APTAIController::EndAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (IsValid(BehaviorTreeComponent))
	{
		BehaviorTreeComponent->StopTree();
	}
}

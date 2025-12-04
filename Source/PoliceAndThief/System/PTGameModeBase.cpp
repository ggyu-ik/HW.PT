#include "System/PTGameModeBase.h"

#include "PTGameStateBase.h"
#include "Controller/PTPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Spawn/AISpawnBox.h"

void APTGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		MainTimerHandle,
		this,
		&ThisClass::OnMainTimerElapsed,
		1.f,
		true
		);
	
	RemainWaitingTimeForPlaying = WaitingTime;
	RemainWaitingTimeForEnding = EndingTime;
}

void APTGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	APTGameStateBase* PTGS = GetGameState<APTGameStateBase>();
	if (!IsValid(PTGS))
	{
		return;
	}
	
	if (PTGS->MatchState != EMatchState::Waiting)
	{
		NewPlayer->SetLifeSpan(0.1f);
		return;
	}
	
	APTPlayerController* PTPC = Cast<APTPlayerController>(NewPlayer);
	if (IsValid(PTPC))
	{
		AlivePlayerControllers.Add(PTPC);
	}
}

void APTGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	
	APTPlayerController* PTPC = Cast<APTPlayerController>(Exiting);
	if (IsValid(PTPC) && AlivePlayerControllers.Find(PTPC) != INDEX_NONE)
	{
		AlivePlayerControllers.Remove(PTPC);
		DeadPlayerControllers.Add(PTPC);
	}
}

void APTGameModeBase::OnCharacterDead(APTPlayerController* InController)
{
	if (!IsValid(InController) || AlivePlayerControllers.Find(InController) == INDEX_NONE)
	{
		return;
	}
	
	InController->ClientRPCShowGameResultWidget(AlivePlayerControllers.Num());
	
	AlivePlayerControllers.Remove(InController);
	DeadPlayerControllers.Add(InController);
}

void APTGameModeBase::OnMainTimerElapsed()
{
	APTGameStateBase* PTGS = GetGameState<APTGameStateBase>();
	if (!IsValid(PTGS))
	{
		return;
	}
	
	switch (PTGS->MatchState)
	{
	case EMatchState::None:
		break;

	case EMatchState::Waiting:
		{
			FString NotificationString = FString::Printf(TEXT(""));
			
			if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
			{
				NotificationString = FString::Printf(TEXT("다른 플레이어를 기다리는중..."));
				
				RemainWaitingTimeForPlaying = WaitingTime;
			}
			else
			{
				NotificationString = FString::Printf(TEXT("게임시작 %d초 전"), RemainWaitingTimeForPlaying - 1);
				--RemainWaitingTimeForPlaying;
			}
			
			if (RemainWaitingTimeForPlaying <= 0)
			{
				NotificationString = FString::Printf(TEXT(""));
				
				PTGS->MatchState = EMatchState::Playing;
			}
			
			NotifyToAllPlayer(NotificationString);
			
			break;
		}
	case EMatchState::Playing:
		{
			if (!bAlreadySpawned)
			{
				if (!IsValid(SpawnBox))
				{
					TArray<AActor*> FoundActors;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAISpawnBox::StaticClass(), FoundActors);
					
					if (FoundActors.Num() > 0)
					{
						SpawnBox = Cast<AAISpawnBox>(FoundActors[0]);
					}
					
					for (auto Player : AlivePlayerControllers)
					{
						if (IsValid(Player) && IsValid(Player->GetPawn()))
						{
							Player->GetPawn()->SetActorLocation(SpawnBox->GetRandomSpawnLocation());
						}
					}
				}
				
				if (IsValid(SpawnBox))
				{
					SpawnBox->SpawnAICharacter(30);
					bAlreadySpawned = true;
				}
			}
			
			PTGS->AlivePlayerControllerCount = AlivePlayerControllers.Num();
			
			FString NotificationString = FString::Printf(TEXT("생존자 %d명"), PTGS->AlivePlayerControllerCount);
			
			NotifyToAllPlayer(NotificationString);
			
			if (PTGS->AlivePlayerControllerCount <= 1)
			{
				AlivePlayerControllers[0]->ClientRPCShowGameResultWidget(1);
				
				PTGS->MatchState = EMatchState::Ending;
			}
		}
		
		break;
		
	case EMatchState::Ending:
		{
			FString NotificationString = FString::Printf(TEXT("타이틀 메뉴까지 %d초 남았습니다."), RemainWaitingTimeForEnding - 1);
			
			NotifyToAllPlayer(NotificationString);
			
			--RemainWaitingTimeForEnding;
			
			if (RemainWaitingTimeForEnding <= 0)
			{
				for (auto Player : AlivePlayerControllers)
				{
					Player->ClientRPCReturnToTitle();
				}
				
				for (auto Player : DeadPlayerControllers)
				{
					Player->ClientRPCReturnToTitle();
				}
				
				MainTimerHandle.Invalidate();
				
				FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
				UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));
			}
		}
		break;
		
	case EMatchState::End:
		break;
		
	default:
		break;
	}
}

void APTGameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto Player : AlivePlayerControllers)
	{
		Player->NotificationText = FText::FromString(NotificationString);
	}
	
	for (auto Player : DeadPlayerControllers)
	{
		Player->NotificationText = FText::FromString(NotificationString);
	}
}

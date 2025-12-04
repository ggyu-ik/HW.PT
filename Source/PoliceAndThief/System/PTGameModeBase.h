#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PTGameModeBase.generated.h"

class AAISpawnBox;
class APTPlayerController;

UCLASS()
class POLICEANDTHIEF_API APTGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void Logout(AController* Exiting) override;
	
	void OnCharacterDead(APTPlayerController* InController);
	
	FTimerHandle MainTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 WaitingTime = 10;
	
	int32 RemainWaitingTimeForPlaying;
	
	int32 MinimumPlayerCountForPlaying = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EndingTime = 10;
	
	int32 RemainWaitingTimeForEnding;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<APTPlayerController>> AlivePlayerControllers;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<APTPlayerController>> DeadPlayerControllers;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<AAISpawnBox> SpawnBox;
	
	uint8 bAlreadySpawned = false;
private:
	void OnMainTimerElapsed();
	
	void NotifyToAllPlayer(const FString& NotificationString);
};

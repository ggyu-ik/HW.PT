#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTPlayerController.generated.h"

class UUserWidget;
class UPTGameResultWidget;

UCLASS()
class POLICEANDTHIEF_API APTPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Client, Reliable)
	void ClientRPCShowGameResultWidget(int32 InRanking);
	
	UFUNCTION(Client, Reliable)
	void ClientRPCReturnToTitle();
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText NotificationText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> NoticationTextUIClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPTGameResultWidget> GameResultUIClass;
};

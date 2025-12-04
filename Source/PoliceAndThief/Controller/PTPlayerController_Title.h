#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PTPlayerController_Title.generated.h"

class UUserWidget;

UCLASS()
class POLICEANDTHIEF_API APTPlayerController_Title : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TitleWidget;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUserWidget> TitleWidgetInstance;
	
public:
	virtual void BeginPlay() override;
	
	void JoinServer(const FString& IP);
};

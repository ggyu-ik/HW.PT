#include "PTPlayerController_Title.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void APTPlayerController_Title::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
	if (IsValid(TitleWidget))
	{
		TitleWidgetInstance = CreateWidget<UUserWidget>(this, TitleWidget);
		if (IsValid(TitleWidgetInstance))
		{
			TitleWidgetInstance->AddToViewport();
			
			FInputModeUIOnly InputModeUIOnly;
			InputModeUIOnly.SetWidgetToFocus(TitleWidgetInstance->GetCachedWidget());
			SetInputMode(InputModeUIOnly);
			
			bShowMouseCursor = true;
		}
	}
}

void APTPlayerController_Title::JoinServer(const FString& IP)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*IP), true);
}

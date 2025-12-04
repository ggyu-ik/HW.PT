#include "PTTitleWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "PoliceAndThief/Controller/PTPlayerController_Title.h"

void UPTTitleWidget::NativeConstruct()
{
	StartButton.Get()->OnClicked.AddDynamic(this, &UPTTitleWidget::OnStartButtonClicked);
	ExitButton.Get()->OnClicked.AddDynamic(this, &UPTTitleWidget::OnExitButtonClicked);
}

void UPTTitleWidget::OnStartButtonClicked()
{
	APTPlayerController_Title* PlayerController = GetOwningPlayer<APTPlayerController_Title>();
	if (IsValid(PlayerController))
	{
		FText ServerIP = ServerIPText->GetText();
		PlayerController->JoinServer(ServerIP.ToString());
	}
}

void UPTTitleWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

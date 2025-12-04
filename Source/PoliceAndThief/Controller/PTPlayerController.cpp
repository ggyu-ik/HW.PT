#include "PTPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/PTGameResultWidget.h"

void APTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsLocalController())
	{
		return;
	}
	
	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
	
	if (IsValid(NoticationTextUIClass))
	{
		UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NoticationTextUIClass);
		if (IsValid(NotificationTextUI))
		{
			NotificationTextUI->AddToViewport(1);
			
			NotificationTextUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void APTPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass, NotificationText);
}

void APTPlayerController::ClientRPCReturnToTitle_Implementation()
{
	if (IsLocalController())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Title")), true);
	}
}

void APTPlayerController::ClientRPCShowGameResultWidget_Implementation(int32 InRanking)
{
	if (IsLocalController())
	{
		if (IsValid(GameResultUIClass))
		{
			UPTGameResultWidget* GameResultUI = CreateWidget<UPTGameResultWidget>(this, GameResultUIClass);
			
			if (IsValid(GameResultUI))
			{
				GameResultUI->AddToViewport(3);
				
				FString GameResultString = FString::Printf(TEXT("%s"), InRanking == 1 ? TEXT("승리했습니다!!") : TEXT("패배했습니다.."));
				GameResultUI->ResultText->SetText(FText::FromString(GameResultString));
				
				FString RankingString = FString::Printf(TEXT("%d등"), InRanking);
				GameResultUI->RankingText->SetText(FText::FromString(RankingString));
				
				FInputModeUIOnly Mode;
				SetInputMode(Mode);
				
				bShowMouseCursor = true;
			}
		}
	}
}

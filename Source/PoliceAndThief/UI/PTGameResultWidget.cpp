#include "UI/PTGameResultWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPTGameResultWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!ReturnToTitleButton.Get()->OnClicked.IsAlreadyBound(this, &ThisClass::OnReturnToTitleButtonClicked))
	{
		ReturnToTitleButton.Get()->OnClicked.AddDynamic(this, &ThisClass::OnReturnToTitleButtonClicked);
	}
}

void UPTGameResultWidget::OnReturnToTitleButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Title")), true);
}

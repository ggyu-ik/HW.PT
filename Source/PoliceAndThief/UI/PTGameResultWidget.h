#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PTGameResultWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class POLICEANDTHIEF_API UPTGameResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UTextBlock> RankingText;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (BindWidget))
	TObjectPtr<UButton> ReturnToTitleButton;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void OnReturnToTitleButtonClicked();
};

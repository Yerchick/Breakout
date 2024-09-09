// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPO_StartGameWidget.generated.h"

class UButton;
class UTextBlock;
class UWidgetSwitcher;

UCLASS()
class BPO_API UBPO_StartGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void ShowAnimalFactInfo(FString text);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OkButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AnimalFactsText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	virtual void NativeOnInitialized() override;

private: 
	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void  ShowAnimaFact();

};

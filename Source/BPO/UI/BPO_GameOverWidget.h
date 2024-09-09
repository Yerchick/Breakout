// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPO_GameOverWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class BPO_API UBPO_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateScore(uint32 points);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PointsText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackToMenuButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnBackToMenu();
};
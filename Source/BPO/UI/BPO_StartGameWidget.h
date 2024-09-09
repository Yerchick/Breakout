// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPO_StartGameWidget.generated.h"

class UButton;

UCLASS()
class BPO_API UBPO_StartGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> OkButton;

	virtual void NativeOnInitialized() override;

private: 
	UFUNCTION()
	void OnStartGame();

};

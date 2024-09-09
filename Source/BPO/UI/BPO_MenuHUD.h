// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BPO_MenuHUD.generated.h"

class UBPO_StartGameWidget;

UCLASS()
class BPO_API ABPO_MenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBPO_StartGameWidget> StartGameWidgetClass;

protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UBPO_StartGameWidget> StartGameWidget;

	UFUNCTION()
	void OnFactReceived(FString fact);

};

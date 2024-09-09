// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BPO_GameplayWidget.generated.h"

class UTextBlock;


UCLASS()
class BPO_API UBPO_GameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	

public :
	void UpdateScore(uint32 points);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PointsText;
};

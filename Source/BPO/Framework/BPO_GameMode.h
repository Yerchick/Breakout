// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BPO/Core/Game.h"
#include "BPO_GameMode.generated.h"


class ABPO_Grid;

UCLASS()
class ABPO_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	public: 
		virtual void StartPlay() override;

protected: 
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "5", ClampMax = "100"))
	FUintPoint GridSize{ 10, 10 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2", ClampMax = "100"))
	FUintPoint CellSize{ 10, 10 };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABPO_Grid> GridVisualClass;


private:
	TUniquePtr<Breakout::Game> Game;

	UPROPERTY()
	ABPO_Grid* GridVisual;
};

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
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "5", ClampMax = "200"))
	FUintPoint GridSize{ 30, 50 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2", ClampMax = "100"))
	FUintPoint CellSize{ 10, 10 };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABPO_Grid> GridVisualClass;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "10"))
	uint8 WallWidth{ 1 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "10"))
	uint8 DeadzoneHeight{ 1 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "200"))
	uint32 PaddleSpeed{ 10 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2", ClampMax = "20"))
	uint8 PaddleWidth{ 4 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "5"))
	uint8 PaddleHeight{ 1 };


private:
	TUniquePtr<Breakout::Game> Game;
	Breakout::Input PaddleInput{ 0 };

	UPROPERTY()
	ABPO_Grid* GridVisual;
};

// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BPO/Core/Game.h"
#include "InputActionValue.h"
#include "BPO_GameMode.generated.h"


class ABPO_Grid;
class ABPO_Paddle;
class UInputAction;
class UInputMappingContext;

UCLASS()
class ABPO_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABPO_GameMode();
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:


	UPROPERTY(EditDefaultsOnly, Category = "Grid Settings")
	TSubclassOf<ABPO_Grid> GridVisualClass;



	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "5", ClampMax = "200"), Category = "Grid Settings")
	FUintPoint GridSize{ 30, 50 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2", ClampMax = "100"), Category = "Grid Settings")
	FUintPoint CellSize{ 10, 10 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "10"), Category = "Grid Settings")
	uint8 WallWidth{ 1 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "10"), Category = "Grid Settings")
	uint8 DeadzoneHeight{ 1 };

	UPROPERTY(EditDefaultsOnly, Category = "Paddle Settings")
	TSubclassOf<ABPO_Paddle> PaddleVisualClass;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "200"), Category = "Paddle Settings")
	uint32 PaddleSpeed{ 10 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "2", ClampMax = "20"), Category = "Paddle Settings")
	uint8 PaddleWidth{ 4 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "5"), Category = "Paddle Settings")
	uint8 PaddleHeight{ 1 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1", ClampMax = "10"), Category = "Game Settings")
	uint8 Difficulty{ 1 };

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.01", ClampMax = "10.0"), Category = "Game Settings")
	float GameSpeed{ 0.5f };

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	TObjectPtr<UInputAction> MoveRight;

	UPROPERTY(EditDefaultsOnly, Category = "Input Settings")
	TObjectPtr <UInputMappingContext> InputMapping;

private:
	TUniquePtr<Breakout::Game> Game;
	Breakout::Input PaddleInput{ 0 };

	UPROPERTY()
	ABPO_Grid* GridVisual;

	UPROPERTY()
	ABPO_Paddle* PaddleVisual;

	void SetupInput();
	void OnMoveRight(const FInputActionValue& Value);
};

// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BPO_HUD.generated.h"

class UBPO_GameplayWidget;
class UBPO_GameOverWidget;


UENUM()
enum class EUIMatchState : uint8
{
	StartGame = 0,
	GameInProgress,
	GameOver,
	GameCompleted
};

namespace Breakout
{
class Game;
}


UCLASS()
class BPO_API ABPO_HUD : public AHUD
{
	GENERATED_BODY()

public:
	void SetModel(const TSharedPtr<Breakout::Game>& Game);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBPO_GameplayWidget> GameplayVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UBPO_GameOverWidget> GameOverVisualClass;



	virtual void BeginPlay() override;

private:


	UPROPERTY()
	TObjectPtr<UBPO_GameplayWidget> GameplayWidget;

	UPROPERTY()
	TObjectPtr<UBPO_GameOverWidget> GameOverWidget;



	UPROPERTY()
	TMap<EUIMatchState, TObjectPtr<UUserWidget>> GameWidgets;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;

	TWeakPtr<Breakout::Game> Game;

	EUIMatchState MatchState;

	void SetUIMatchState(EUIMatchState NewMatchState);
};

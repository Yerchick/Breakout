// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_HUD.h"
#include "UI/BPO_GameplayWidget.h"
#include "UI/BPO_GameOverWidget.h"
#include "UI/BPO_StartGameWidget.h"
#include "Core/Game.h"

void ABPO_HUD::BeginPlay()
{
	Super::BeginPlay();

	GameplayWidget = CreateWidget<UBPO_GameplayWidget>(GetWorld(), GameplayVisualClass);
	check(GameplayWidget);
	GameWidgets.Add(EUIMatchState::GameInProgress, GameplayWidget);

	GameOverWidget = CreateWidget<UBPO_GameOverWidget>(GetWorld(), GameOverVisualClass);
	check(GameOverWidget);
	GameWidgets.Add(EUIMatchState::GameOver, GameOverWidget);

	
	GameWidgets.Add(EUIMatchState::StartGame, GameOverWidget);

	for (auto& [UIState, GameWidget] : GameWidgets) {
		if (GameWidget) {
			GameWidget->AddToViewport();
			GameWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}



void ABPO_HUD::SetModel(const TSharedPtr<Breakout::Game>& inGame)
{
	if (!inGame) return;

	Game = inGame;

	SetUIMatchState(EUIMatchState::GameInProgress);

	GameplayWidget->UpdateScore(inGame->score());
	GameOverWidget->UpdateScore(inGame->score());

	using namespace Breakout;
	inGame->subscribeOnGameplayEvent([&](GameplayEvent Event)
		{
			switch (Event) {
				case GameplayEvent::BlockBroken:
				case GameplayEvent::SuperBlockBroken:
					GameplayWidget->UpdateScore(Game.Pin()->score());
					break;
				case GameplayEvent::GameOver:
				case GameplayEvent::GameCompleted:
					GameOverWidget->UpdateScore(inGame->score());
					SetUIMatchState(EUIMatchState::GameOver);
					break;
				default:
					break;
			}
		});
}

void ABPO_HUD::SetUIMatchState(EUIMatchState NewMatchState)
{
	if (CurrentWidget) {
		CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (GameWidgets.Contains(NewMatchState)) {
		CurrentWidget = GameWidgets[NewMatchState];
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
	}
}
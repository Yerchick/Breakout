// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Framework/BPO_GameMode.h"
#include "BPO_GameMode.h"
#include "BPO/Core/BPO_Types.h"
#include "BPO/Core/Grid.h"
#include "World/BPO_Grid.h"
#include "Framework/BPO_Pawn.h"

using namespace Breakout;

void ABPO_GameMode::StartPlay()
{
	Super::StartPlay();

	// ini core game
	Breakout::Settings GS;
	GS.difficulty = 1;
	GS.grid.gridSize = Breakout::Dim{ GridSize.X, GridSize.Y };
	GS.grid.wallWidth = WallWidth;
	GS.grid.deadzoneHeight = DeadzoneHeight;
	GS.paddle.speed = PaddleSpeed;
	GS.paddle.height = PaddleHeight;
	GS.paddle.width = PaddleWidth;
	GS.paddle.startPosition = Breakout::Position{ GridSize.X / 2 + PaddleWidth/2, DeadzoneHeight};

	Game = MakeUnique<Breakout::Game>(GS);
	check(Game.IsValid());

	// init world grid
	const FTransform GridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ABPO_Grid>(GridVisualClass, GridOrigin);
	check(GridVisual);
	GridVisual->SetModel(Game->grid(), CellSize);
	GridVisual->FinishSpawning(GridOrigin);

	// set pawn location fitting grid in viewport
	auto* PC = GetWorld()->GetFirstPlayerController();
	check(PC);

	auto* Pawn = Cast<ABPO_Pawn>(PC->GetPawn());
	check(Pawn);
	check(Game->grid().IsValid());
	Pawn->UpdateLocation(Game->grid()->dim(), CellSize, GridOrigin);
}

void ABPO_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Game.IsValid()) {
		Game->update(DeltaSeconds, PaddleInput);
	}
}

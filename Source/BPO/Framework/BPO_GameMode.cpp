// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Framework/BPO_GameMode.h"
#include "BPO_GameMode.h"
#include "BPO/Core/BPO_Types.h"
#include "BPO/Core/Grid.h"
#include "World/BPO_Grid.h"
#include "World/BPO_Paddle.h"
#include "Framework/BPO_Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

using namespace Breakout;

ABPO_GameMode::ABPO_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABPO_GameMode::StartPlay()
{
	Super::StartPlay();

	// ini core game
	Breakout::Settings GS;
	GS.difficulty = Difficulty;
	GS.gameSpeed = GameSpeed;
	GS.grid.gridSize = Breakout::Dim{ GridSize.X, GridSize.Y };
	GS.grid.wallWidth = WallWidth;
	GS.grid.deadzoneHeight = DeadzoneHeight;
	GS.paddle.speed = PaddleSpeed;
	GS.paddle.height = PaddleHeight;
	GS.paddle.width = PaddleWidth;
	GS.paddle.startPosition = Breakout::Position{ GridSize.X / 2 + PaddleWidth / 2, GridSize.Y + WallWidth };

	Game = MakeUnique<Breakout::Game>(GS);
	check(Game.IsValid());

	// init world grid
	const FTransform GridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ABPO_Grid>(GridVisualClass, GridOrigin);
	check(GridVisual);
	GridVisual->SetModel(Game->grid(), CellSize, WallWidth);
	GridVisual->FinishSpawning(GridOrigin);

	//init world paddle
	PaddleVisual = GetWorld()->SpawnActorDeferred<ABPO_Paddle>(PaddleVisualClass, GridOrigin);
	PaddleVisual->SetModel(Game->paddle(), CellSize, FUintPoint{ PaddleWidth , PaddleHeight }, Game->grid()->dim());
	PaddleVisual->FinishSpawning(GridOrigin);

	// set pawn location fitting grid in viewport
	auto* PC = GetWorld()->GetFirstPlayerController();
	check(PC);

	auto* Pawn = Cast<ABPO_Pawn>(PC->GetPawn());
	check(Pawn);
	check(Game->grid().IsValid());
	Pawn->UpdateLocation(Game->grid()->dim(), CellSize, GridOrigin);

	//
	SetupInput();
}

void ABPO_GameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (Game.IsValid()) {
		Game->update(DeltaSeconds, PaddleInput);
	}
}

void ABPO_GameMode::SetupInput()
{
	if (!GetWorld()) return;

	if (auto* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())) {
		if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer())) {
			InputSystem->AddMappingContext(InputMapping, 0);
		}

		auto* Input = Cast<UEnhancedInputComponent>(PC->InputComponent);
		check(Input);
		Input->BindAction(MoveRight, ETriggerEvent::Triggered, this, &ThisClass::OnMoveRight);
	}
}

void ABPO_GameMode::OnMoveRight(const FInputActionValue& Value)
{
	float inputValue = FMath::Clamp(Value.Get<float>(), -1, 1);
	UE_LOG(LogTemp, Display, TEXT("OnMoveRight received input: %f"), inputValue);
	PaddleInput = Breakout::Input(inputValue);
	
}

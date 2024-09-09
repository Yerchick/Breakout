// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Framework/BPO_GameMode.h"
#include "BPO_GameMode.h"
#include "BPO/Core/BPO_Types.h"
#include "BPO/Core/Grid.h"
#include "World/BPO_Grid.h"
#include "World/BPO_Paddle.h"
#include "World/BPO_Ball.h"
#include "World/BPO_Block.h"
#include "Framework/BPO_Pawn.h"
#include "UI/BPO_HUD.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"



using namespace Breakout;

DEFINE_LOG_CATEGORY_STATIC(LogBPOGameMode, All, All)

ABPO_GameMode::ABPO_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABPO_GameMode::StartPlay()
{
	Super::StartPlay();
	
	check(SuperBlockVisualClass);
	check(BlockVisualClass);

	// ini core game
	Game = MakeShared<Breakout::Game>(MakeSettings());
	check(Game.IsValid());

	// init world grid
	const FTransform GridOrigin = FTransform::Identity;
	check(GetWorld());
	GridVisual = GetWorld()->SpawnActorDeferred<ABPO_Grid>(GridVisualClass, GridOrigin);
	check(GridVisual);
	GridVisual->SetModel(Game->grid(), CellSize, WallWidth);
	GridVisual->FinishSpawning(GridOrigin);

	SusbcribeOnGameEvents();

	InitWorldBlocks();

	//init world paddle
	PaddleVisual = GetWorld()->SpawnActorDeferred<ABPO_Paddle>(PaddleVisualClass, GridOrigin);
	PaddleVisual->SetModel(Game->paddle(), CellSize, FUintPoint{ PaddleWidth , PaddleHeight }, Game->grid()->dim());
	PaddleVisual->FinishSpawning(GridOrigin);

	//init world ball
	BallVisual = GetWorld()->SpawnActorDeferred<ABPO_Ball>(BallVisualClass, GridOrigin);
	BallVisual->SetModel(Game->ball(), CellSize, Game->grid()->dim());
	BallVisual->FinishSpawning(GridOrigin);

	// set pawn location fitting grid in viewport
	auto* PC = GetWorld()->GetFirstPlayerController();
	check(PC);

	auto* Pawn = Cast<ABPO_Pawn>(PC->GetPawn());
	check(Pawn);
	check(Game->grid().IsValid());
	Pawn->UpdateLocation(Game->grid()->dim(), CellSize, GridOrigin);

	//
	SetupInput();

	//
	BPO_HUD = Cast<ABPO_HUD>(PC->GetHUD());
	check(BPO_HUD);
	BPO_HUD->SetModel(Game);
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
			InputSystem->AddMappingContext(InputPaddleMapping, 0);
			InputSystem->AddMappingContext(InputGameMapping, 0);
		}

		auto* Input = Cast<UEnhancedInputComponent>(PC->InputComponent);
		check(Input);
		Input->BindAction(MoveRight, ETriggerEvent::Triggered, this, &ThisClass::OnMoveRight);
		Input->BindAction(SpeedUp, ETriggerEvent::Triggered, this, &ThisClass::OnSpeedUp);
		Input->BindAction(Restart, ETriggerEvent::Triggered, this, &ThisClass::OnRestart);
	}
}

void ABPO_GameMode::OnMoveRight(const FInputActionValue& Value)
{
	float inputValue = FMath::Clamp(Value.Get<float>(), -1, 1);
	PaddleInput = Breakout::Input(inputValue);

}

void ABPO_GameMode::OnSpeedUp(const FInputActionValue& Value)
{
	const bool inputValue = Value.Get<bool>();
	//	PaddleInput = Breakout::Input(inputValue);
	check(Game.IsValid());
	Game.Get()->updateGameSpeed(inputValue ? GameSpeed * 5: GameSpeed);
}

void ABPO_GameMode::OnRestart(const FInputActionValue& Value)
{
	if (const bool inputValue = Value.Get<bool>()) {
		Game = MakeShared<Breakout::Game>(MakeSettings());
		check(Game.IsValid());
		SusbcribeOnGameEvents();
		GridVisual->SetModel(Game->grid(), CellSize, WallWidth);
		InitWorldBlocks();
		PaddleVisual->SetModel(Game->paddle(), CellSize, FUintPoint{ PaddleWidth , PaddleHeight }, Game->grid()->dim());
		BallVisual->SetModel(Game->ball(), CellSize, Game->grid()->dim());
		BPO_HUD->SetModel(Game);
	}
}

void ABPO_GameMode::InitWorldBlocks()
{
	//init world blocks
	auto blocks = Game->blocks();
	for (int i = 0; i < blocks.Num(); ++i) {
		auto model = blocks[i];
		BlockType type = model.Get()->type();
		TSubclassOf<ABPO_Block> blockClass = type == BlockType::Super ? SuperBlockVisualClass : BlockVisualClass;
		auto newBlock = GetWorld()->SpawnActorDeferred<ABPO_Block>(blockClass, FTransform::Identity);
		
		newBlock->SetModel(model, CellSize, BlockSize, Game->grid()->dim());
		newBlock->FinishSpawning(FTransform::Identity);
		BlocksVisual.Add(newBlock);
	}
}

Breakout::Settings ABPO_GameMode::MakeSettings() const
{
	Breakout::Settings GS;
	GS.difficulty = Difficulty;
	GS.gameSpeed = GameSpeed;
	GS.grid.cellSize = CellSize;
	GS.grid.blockSize = BlockSize;
	GS.ballSpeed = BallSpeed;
	GS.grid.gridSize = Breakout::Dim{ GridSize.X, GridSize.Y };
	GS.grid.wallWidth = WallWidth;
	GS.grid.deadzoneHeight = DeadzoneHeight;
	GS.paddle.speed = PaddleSpeed;
	GS.paddle.height = PaddleHeight;
	GS.paddle.width = PaddleWidth;
	GS.paddle.startPosition = Breakout::Position{ (GridSize.X+ WallWidth) / 2 + PaddleWidth / 2, GridSize.Y + WallWidth };
	return GS;
}


void ABPO_GameMode::SusbcribeOnGameEvents()
{
	using namespace Breakout;

	Game->subscribeOnGameplayEvent([](GameplayEvent Event)
		{
			switch (Event) {
				case GameplayEvent::GameOver:
					UE_LOG(LogBPOGameMode, Display, TEXT("----------------Game Over!-------------------"));

					break;
				case GameplayEvent::GameCompleted:
					UE_LOG(LogBPOGameMode, Display, TEXT("----------------Game Completed!-------------------"));

					break;
				default:
					break;
			}
		});
}
// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h" 

namespace Breakout
{

class Grid;
class Paddle;
class Ball;
class Block;

class Game
{
public:
	Game(const Settings& settings);
	TSharedPtr<Grid> grid() const { return m_grid; };
	TSharedPtr<Paddle> paddle() const { return m_paddle; };
	TSharedPtr<Ball> ball() const { return m_ball; };
	TArray<TSharedPtr<Block>>  blocks() const { return m_blocks; };
	void update(float deltaSeconds, const Input& input);
	void updateGameSpeed(float newSpeed);

	uint32 score() const { return m_score; }

	void subscribeOnGameplayEvent(GameplayEventCallback callback);

private:
	const Settings c_settings;
	TSharedPtr<Grid> m_grid;
	TSharedPtr<Paddle> m_paddle;
	TSharedPtr<Ball> m_ball;
	TArray<TSharedPtr<Block>> m_blocks;

	TArray<GameplayEventCallback> m_gameplayEventCallbacks;
	void dispatchEvent(GameplayEvent Event);

	void movePaddle(const Input& input);
	bool canPaddleMove(const Input& input);
	void moveBall(const float deltaSeconds, const float ballSpeed);
	void updateGrid();
	void onBlockHit(const Position& pos, bool superBlock = false);
	//TArray<Breakout::Position> getAllIntersectingPositions(FVector2D atPoint, float radius, CellType cellType);
	//Breakout::Position getNearestIntersectionPoint(FVector2D atPoint, TArray<Breakout::Position> positions);
	TArray<Breakout::IntersectionResult> getAllIntersectingResults(FVector2D atPoint, float radius);
	Breakout::IntersectionResult getNearestIntersectionResult(FVector2D atPoint, TArray<Breakout::IntersectionResult> results);
	void UpdateDirrectionAfterHit(Breakout::IntersectionResult hit, FVector2D& dirrection);
	//FVector2D getDirrectionAfterHit(FVector2D pos, FVector2D oldPos, FVector2D dirrection);

	//FUintRect freeGamingSpace;
	float m_movePaddleSeconds{0.0f};
	float m_gameSpeed{0.0f};
	float m_ballSpeed{0.0f};
	FVector2D m_ballDirrection{1, -1};
	bool isTimeToUpdate(float deltaSeconds);
	bool m_gameOver = false;
	bool m_gridChanged = false;
	uint32 m_score{ 0 };

	void gameOver();
	void generateBlocks(FUintRect freeGamingSpace);

};
} //namespace Breakout


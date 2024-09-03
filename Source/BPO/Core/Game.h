// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h" 

namespace Breakout
{

class Grid;
class Paddle;
class Ball;

class Game
{
public:
	Game(const Settings& settings);
	TSharedPtr<Grid> grid() const { return m_grid; };
	TSharedPtr<Paddle> paddle() const { return m_paddle; };
	void update(float deltaSeconds, const Input& input);

private:
	const Settings c_settings;
	TSharedPtr<Grid> m_grid;
	TSharedPtr<Paddle> m_paddle;
	TSharedPtr<Ball> m_ball;

	void movePaddle(const Input& input);
	bool canPaddleMove(const Input& input);
	void moveBall(const float deltaSeconds);
	void updateGrid();

	float m_movePaddleSeconds{0.0f};
	bool isTimeToUpdate(float deltaSeconds);
	bool m_gameOver = false;
	bool m_gridChanged = false;

};
} //namespace Breakout


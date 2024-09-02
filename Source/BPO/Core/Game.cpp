// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Game.h"
#include "BPO/Core/Grid.h"
#include "BPO/Core/Paddle.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All)

using namespace Breakout;

Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings);
	m_paddle = MakeShared<Paddle>(settings);

	m_grid->update(m_paddle->body(), CellType::Paddle);
	m_grid->printDebug();
}

void Game::update(float deltaSeconds, const Input& input)
{
	//
	movePaddle(input);
}

void Game::movePaddle(const Input& input)
{
	m_paddle->move(input);
}


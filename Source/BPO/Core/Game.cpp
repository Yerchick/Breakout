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
	m_gridChanged = true;
	updateGrid();
}

void Game::update(float deltaSeconds, const Input& input)
{
	//
	if (!isTimeToUpdate(deltaSeconds)) return;


	movePaddle(input);
	moveBall(deltaSeconds);
	updateGrid();
}

void Game::movePaddle(const Input& input)
{
	if (canPaddleMove(input)) {
		m_paddle->move(input);
		m_gridChanged = true;
	}
}

bool Breakout::Game::canPaddleMove(const Input& input)
{
	if (input.x == 0) return false;
	Position pos{0,0};
	if (input.x > 0) {
		pos = Position{ 1 , 0 } + m_paddle->head();
	} else {
		pos =  m_paddle->tail();
	}
	//UE_LOG(LogGame, Display, TEXT("canPaddleMove, scaning position: %d + input: %d"), pos.x, (uint32)input.x);
	return m_grid->hitTest(pos, CellType::PaddleZone);
}

void Game::moveBall(const float deltaSeconds)
{

}

void Game::updateGrid()
{
	if (!m_gridChanged) return;
	//UE_LOG(LogGame, Display, TEXT("UpdateGrid with a paddle: %d, %d"), m_paddle->tail().x, m_paddle->head().x);
	m_grid->update(m_paddle->body(), CellType::Paddle, true);
	m_grid->printDebug();
	m_gridChanged = false;
}

bool Game::isTimeToUpdate(float deltaSeconds)
{
	m_movePaddleSeconds += deltaSeconds;
	if(m_movePaddleSeconds < c_settings.gameSpeed)	return false;
	m_movePaddleSeconds = 0.0f;
	return true;
}

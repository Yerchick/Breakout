// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Game.h"
#include "BPO/Core/Grid.h"
#include "BPO/Core/Paddle.h"
#include "BPO/Core/Ball.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All)

using namespace Breakout;

Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings);
	m_paddle = MakeShared<Paddle>(settings);
	m_ball = MakeShared<Ball>(settings);
	m_ballSpeed = settings.ballSpeed;
	m_gridChanged = true;
	m_gameSpeed = settings.gameSpeed;
	updateGrid();
}

void Game::updateGameSpeed(float newSpeed)
{
	m_gameSpeed = newSpeed;
}

void Game::update(float deltaSeconds, const Input& input)
{
	if (m_gameOver) {		return;	}

	if (died()) {
		UE_LOG(LogGame, Display, TEXT("----------------GameOver!-------------------"));
		m_gameOver = true;
	}
	//
	if (isTimeToUpdate(deltaSeconds)) {
		movePaddle(input);
	}
	moveBall(deltaSeconds, m_ballSpeed);
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
	Position pos{ 0,0 };
	if (input.x > 0) {
		pos = Position{ 1 , 0 } + m_paddle->head();
	} else {
		pos = m_paddle->tail();
	}
	//UE_LOG(LogGame, Display, TEXT("canPaddleMove, scaning position: %d + input: %d"), pos.x, (uint32)input.x);
	return m_grid->hitTest(pos, CellType::PaddleZone);
}

void Game::moveBall(const float deltaSeconds, const float ballSpeed)
{
	bool positionFound = false;
	FVector2D delta;
	Position oldPos = m_ball->body()->GetValue();
	FVector2D oldAccuratePos;
	FVector2D newPos;
	Position potentialNewPos{ 0,0 };

	while (!positionFound) {
		delta = (m_ballDirrection * deltaSeconds * ballSpeed* m_gameSpeed);
		oldPos = m_ball->body()->GetValue();
		oldAccuratePos = m_ball->positionAccurate();
		newPos = oldAccuratePos + delta;
		potentialNewPos = m_ball->getRoundedPosition(newPos);
		FVector2D dirrectionMultiplier = FVector2D::One();
		FVector2D dirrectionAdditive = FVector2D::Zero();

		CellType hit = m_grid->hitResult(potentialNewPos);
		switch (m_grid->hitResult(potentialNewPos)) {
			case CellType::Error:

				break;
			case CellType::Empty:

				break;
			case CellType::Wall:
				//in which dirrection hit
			//right
				{
					bool rightWall = m_grid->hitTest(Position(1, 0) + oldPos, CellType::Wall);
					//left
					bool leftWall = m_grid->hitTest(Position(-1, 0) + oldPos, CellType::Wall);
					//ceiling
					bool ceiling = m_grid->hitTest(Position(0, -1) + oldPos, CellType::Wall);
					//bottom

					dirrectionMultiplier = FVector2D(rightWall || leftWall ? -1 : 1, ceiling ? -1 : 1);
				}
				break;
			case CellType::Paddle:
				{
					const auto& Links = m_paddle.Get()->links();

					uint32 i = 0;
					for (const auto& Link : Links) {
						if (Link.x == potentialNewPos.x) {
							UE_LOG(LogGame, Display, TEXT("Hit paddle at index: %d, in order: %f, after correction: %f, and multiplier: %f"), i, i / (float)Links.Num(), (i / (float)Links.Num()) - 0.5f, ((i / (float)Links.Num()) - 0.5f) * 2);
							dirrectionAdditive = FVector2D(((i / (float)Links.Num()) - 0.5f) * -2, 0); //Hit on a paddle changes angle
							dirrectionMultiplier = FVector2D(1, -1); //Hit on a paddle changes dirrection
							//break;
						}
						++i;
					}
				}
				break;
			case CellType::DeadZone:

				UE_LOG(LogGame, Display, TEXT("----------------GameOver!-------------------"));
				m_gameOver = true;
				break;
			default:

				break;
				
		}



		//if (m_grid->hitTest(potentialNewPos, CellType::Wall)) {
		//	//in which dirrection hit
		//	//right
		//	bool rightWall = m_grid->hitTest(Position(1, 0) + oldPos, CellType::Wall);
		//	//left
		//	bool leftWall = m_grid->hitTest(Position(-1, 0) + oldPos, CellType::Wall);
		//	//ceiling
		//	bool ceiling = m_grid->hitTest(Position(0, -1) + oldPos, CellType::Wall);
		//	//bottom

		//	dirrectionMultiplier = FVector2D(rightWall || leftWall ? -1 : 1, ceiling ? -1 : 1);

		//} else if (m_grid->hitTest(potentialNewPos, CellType::Paddle)) {
		//	const auto& Links = m_paddle.Get()->links();

		//	uint32 i = 0;
		//	for (const auto& Link : Links) {
		//		if (Link.x == potentialNewPos.x) {
		//			UE_LOG(LogGame, Display, TEXT("Hit paddle at index: %d, in order: %f, after correction: %f, and multiplier: %f"), i, i / (float)Links.Num(), (i / (float)Links.Num()) - 0.5f, ((i / (float)Links.Num()) - 0.5f) * 2);
		//			dirrectionAdditive = FVector2D(((i / (float)Links.Num())-0.5f)*-2, 0); //Hit on a paddle changes angle
		//			dirrectionMultiplier = FVector2D(1, -1); //Hit on a paddle changes dirrection
		//			break;
		//		}
		//		++i;
		//	}

		//	//dirrectionMultiplier = FVector2D(1, -1);
		//	UE_LOG(LogGame, Display, TEXT("Ball Dirrection Multiplier: %f, %f"), dirrectionMultiplier.X, dirrectionMultiplier.Y);
		//	UE_LOG(LogGame, Display, TEXT("Ball Dirrection Additive: %f, %f"), dirrectionAdditive.X, dirrectionAdditive.Y);
		//} else if (m_grid->hitTest(potentialNewPos, CellType::DeadZone)) {
		//	
		//} 

		positionFound = true;
		//m_ballDirrection = m_ballDirrection * dirrectionMultiplier + dirrectionAdditive;
		//UE_LOG(LogGame, Display, TEXT("Ball Dirrection: %f, %f"), m_ballDirrection.X, m_ballDirrection.Y);
		m_ballDirrection = m_ballDirrection * dirrectionMultiplier + dirrectionAdditive;
		m_ballDirrection = FVector2D(FMath::Clamp(m_ballDirrection.X, -1, 1), FMath::Clamp(m_ballDirrection.Y, -1, 1));
		//UE_LOG(LogGame, Display, TEXT("Update Ball Dirrection: %f, %f"), m_ballDirrection.X, m_ballDirrection.Y);
	}
	
	m_ball->correctPosition(delta);
	if (oldPos != newPos) {
		m_ball->move(potentialNewPos);
		m_gridChanged = true;
	} 
}



void Game::updateGrid()
{
	if (!m_gridChanged) return;
	//UE_LOG(LogGame, Display, TEXT("UpdateGrid with a paddle: %d, %d"), m_paddle->tail().x, m_paddle->head().x);
	m_grid->update(m_paddle->body(), CellType::Paddle, true);
	m_grid->update(m_ball->body(), CellType::Ball, true);
	//m_grid->printDebug();
	m_gridChanged = false;
}

bool Game::isTimeToUpdate(float deltaSeconds)
{
	m_movePaddleSeconds += deltaSeconds;
	if (m_movePaddleSeconds < c_settings.gameSpeed)	return false;
	m_movePaddleSeconds = 0.0f;
	return true;
}

bool Breakout::Game::died() const
{
	UE_LOG(LogGame, Display, TEXT("Check to die: %d, %d"), m_ball->body()->GetValue().x, m_ball->body()->GetValue().y);
	//return m_grid->hitTest(m_ball->body()->GetValue(), CellType::DeadZone);
	return false;
}

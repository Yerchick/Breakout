// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Game.h"
#include "BPO/Core/Grid.h"
#include "BPO/Core/Paddle.h"
#include "BPO/Core/Ball.h"
#include "BPO/Core/Block.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All)

using namespace Breakout;

Game::Game(const Settings& settings) : c_settings(settings)
{
	m_grid = MakeShared<Grid>(settings);
	generateBlocks(m_grid.Get()->getFreeSpace());
	m_paddle = MakeShared<Paddle>(settings);
	m_ball = MakeShared<Ball>(settings);
	m_ballSpeed = settings.ballSpeed;
	m_gridChanged = true;
	m_gameSpeed = settings.gameSpeed;
	updateGrid();
}

void Game::generateBlocks(FUintRect freeGamingSpace)
{
	m_blocks.Empty();
	for (uint32 y = freeGamingSpace.Min.Y; y <= freeGamingSpace.Max.Y * 0.7; ++y) {
		for (uint32 x = freeGamingSpace.Min.X; x <= freeGamingSpace.Max.X; ++x) {
			if (FMath::RandRange(0, 100) > 70) {
				auto block = MakeShared<Block>();
				block.Get().setPossitions(TArray<Position>{ {x, y}});
				m_blocks.Add(block);
			}

		}
	}
	m_grid->update(m_blocks, CellType::Block);
}

void Game::updateGameSpeed(float newSpeed)
{
	m_gameSpeed = newSpeed;
}

void Game::update(float deltaSeconds, const Input& input)
{
	if (m_gameOver) { return; }

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

bool Game::canPaddleMove(const Input& input)
{
	if (input.x == 0) return false;
	Position pos{ 0,0 };
	if (input.x > 0) {
		pos = Position{ 1 , 0 } + m_paddle->head();
	} else {
		pos = m_paddle->tail();
	}
	return m_grid->hitTest(pos, CellType::PaddleZone);
}

void Game::moveBall(const float deltaSeconds, const float ballSpeed)
{
	FVector2D delta = m_ballDirrection * deltaSeconds * ballSpeed * m_gameSpeed;
	Position oldPos = m_ball->getPosition();

	FVector2D oldAccuratePos = m_ball->positionAccurate();
	FVector2D newPos;
	Position potentialNewPos{ 0,0 };
	FVector2D deltaSummary = m_ball->deltaAccurate() + delta;
	if ((deltaSummary).Length() >= 1) {
		FVector2D tempDirrection = m_ballDirrection;
		FVector2D tempOldPos = oldAccuratePos;
		FVector2D tempdelta = delta;
		for (int i = 0; i < deltaSummary.Length(); ++i) {
			auto pos = tempOldPos + i / deltaSummary.Length() * tempdelta;
			tempDirrection = getDirrectionAfterHit(pos, tempOldPos, tempDirrection);
			tempOldPos = pos;
			tempdelta = tempDirrection * deltaSeconds * ballSpeed * m_gameSpeed;
			continue;
		}
		oldAccuratePos = tempOldPos;
		m_ballDirrection = tempDirrection;

		delta = (m_ballDirrection * deltaSeconds * ballSpeed * m_gameSpeed);
	}

	newPos = oldAccuratePos + m_ball->deltaAccurate() + delta;
	potentialNewPos = m_ball->getRoundedPosition(newPos);

	m_ballDirrection = getDirrectionAfterHit(newPos, oldAccuratePos, m_ballDirrection);
	if (oldPos != potentialNewPos) {
		m_ball->move(potentialNewPos, newPos);
		//m_gridChanged = true;
	} else {
		m_ball->correctPosition(delta);
	}
}

FVector2D Game::getDirrectionAfterHit(FVector2D pos, FVector2D oldPos, FVector2D dirrection)
{
	//UE_LOG(LogGame, Display, TEXT("getDirrectionAfterHit: %s, %s, %s"), ToString(m_grid->hitResult(pos)), ToString(m_grid->hitCellType(pos)), *pos.ToString());
	FVector2D resultDirrection = dirrection;
	switch (m_grid->hitResult(pos)) {
		case HitResult::Changes:
			switch (m_grid->hitCellType(pos)) {
				case CellType::Block:
					{
						bool rightWall = m_grid->hitTest(Position(1, 0) + oldPos, CellType::Block);
						bool leftWall = m_grid->hitTest(Position(-1, 0) + oldPos, CellType::Block);
						bool ceiling = m_grid->hitTest(Position(0, -1) + oldPos, CellType::Block);
						bool floor = m_grid->hitTest(Position(0, 1) + oldPos, CellType::Block);

						resultDirrection *= FVector2D(rightWall || leftWall ? -1 : 1, ceiling || floor ? -1 : 1);
						UE_LOG(LogGame, Display, TEXT("Changed dirrection from: %s, to: %s"), *dirrection.ToString(), *resultDirrection.ToString());
						onBlockHit(pos);
					} // No break! 
				case CellType::Wall:
					{
						bool rightWall = m_grid->hitTest(Position(1, 0) + oldPos, CellType::Wall);
						bool leftWall = m_grid->hitTest(Position(-1, 0) + oldPos, CellType::Wall);
						bool ceiling = m_grid->hitTest(Position(0, -1) + oldPos, CellType::Wall);

						resultDirrection *= FVector2D(rightWall || leftWall ? -1 : 1, ceiling ? -1 : 1);
						UE_LOG(LogGame, Display, TEXT("Changed dirrection from: %s, to: %s"), *dirrection.ToString(), *resultDirrection.ToString());

					}
					break;
				case CellType::Paddle:
					{
						const auto& Links = m_paddle.Get()->links();
						uint32 index = 0;
						//	UE_LOG(LogGame, Display, TEXT("Paddle links num: %d"), Links.Num());

						for (const auto& Link : Links) {
							//		UE_LOG(LogGame, Display, TEXT("Paddle link num: %d, with index: %d"), Link.x, index);
							if (Link.x == (int32)(pos.X + 0.5)) {
								resultDirrection += FVector2D(((index / (float)Links.Num()) - 0.5f) * -2, 0); //Hit on a paddle changes angle
								resultDirrection *= FVector2D(1, -1); //Hit on a paddle changes dirrection
								UE_LOG(LogGame, Display, TEXT("-------------------- \n Changed dirrection from : % s, to : % s"), *dirrection.ToString(), *resultDirrection.ToString());
								return resultDirrection;
								//break;
							}
							++index;
						}
					}
					break;


				case CellType::DeadZone:
					{
						UE_LOG(LogGame, Display, TEXT("----------------GameOver!-------------------"));
						m_gameOver = true;
					}

					break;
				default:
					//UE_LOG(LogGame, Display, TEXT("Temp dirrection: %f, %f"), tempDirrection.X, tempDirrection.Y);
					break;
			}

			break;
		case HitResult::Fault:
			UE_LOG(LogGame, Display, TEXT("----------------GameOver!-------------------"));
			m_gameOver = true;
			break;
		case HitResult::Success:

			break;

	}
	return resultDirrection;
}

void Game::onBlockHit(const Position& pos)
{
	int32 blockIndex = -1;
	TSharedPtr<Breakout::Block> blockPtr;
	for (int i = 0; m_blocks.Num(); ++i) {
		if (m_blocks.IsValidIndex(i)) {
			blockPtr = m_blocks[i];
			if (blockPtr.Get()->positions()[0] == pos) {
				blockIndex = i;
				break;
			}
		}
	}
	if (blockIndex != -1 && blockPtr.IsValid()) {
		blockPtr.Reset();
		m_blocks.RemoveAt(blockIndex);
		m_grid->update(pos, CellType::Empty);
		//Add points for player
		m_gridChanged = true;
	}
}

void Game::updateGrid()
{
	if (!m_gridChanged) return;
	//UE_LOG(LogGame, Display, TEXT("UpdateGrid with a paddle: %d, %d"), m_paddle->tail().x, m_paddle->head().x);
	m_grid->update(m_paddle->body(), CellType::Paddle, true);
	m_grid->update(m_ball->body(), CellType::Ball, true);
	m_grid->update(m_blocks, CellType::Block);
	m_grid->printDebug();
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
	//UE_LOG(LogGame, Display, TEXT("Check to die: %d, %d"), m_ball->body()->GetValue().x, m_ball->body()->GetValue().y);
	//return m_grid->hitTest(m_ball->body()->GetValue(), CellType::DeadZone);
	return false;
}

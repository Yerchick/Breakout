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
			if (FMath::RandRange(0, 100) > 99) {
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

	newPos = m_ball->positionAccurate() + m_ball->deltaAccurate() + delta;
	potentialNewPos = m_ball->getRoundedPosition(newPos);

	float ballRadius = c_settings.grid.cellSize / 2;

	auto points = getAllIntersectingResults(newPos, 0.5);
	if (!points.IsEmpty()) {
		auto intersectionResult = getNearestIntersectionResult(newPos, points);
		UpdateDirrectionAfterHit(intersectionResult, m_ballDirrection);
	}
	m_ball->move(potentialNewPos, newPos);
}

TArray<Breakout::IntersectionResult> Game::getAllIntersectingResults(FVector2D atPoint, float radius)
{
	TArray<Breakout::IntersectionResult> results;
	TArray<FVector2D> ballBorders;

	if (m_ballDirrection.Y > 0) {
		ballBorders.Add(atPoint + FVector2D(0, radius));
	} else {
		ballBorders.Add(atPoint + FVector2D(0, -radius));
	}
	if (m_ballDirrection.X > 0) {

		ballBorders.Add(atPoint + FVector2D(radius, 0));
			} else {
		ballBorders.Add(atPoint + FVector2D(-radius, 0));
	}

	for (FVector2D border : ballBorders) {
		Breakout::IntersectionResult result;
		result.ballPoint = atPoint;
		CellType type = CellType::Empty;
		result.pos = m_ball->getRoundedPosition(border);
		result.hitPoint = border;
		result.type = m_grid->hitCellType(result.pos);
		if (result.type != CellType::Empty && result.type != CellType::PaddleZone) {
			UE_LOG(LogGame, Display, TEXT("Adding intersection result with pos: %s, at point: %s = %d, %d,  with: %s, at: %s"),
				*atPoint.ToString(), *border.ToString(), result.pos.x, result.pos.y, ToString(result.type), ToString(result.dirr));
			results.Add(result);
		}
	}
	return results;
}

Breakout::IntersectionResult Game::getNearestIntersectionResult(FVector2D atPoint, TArray<Breakout::IntersectionResult> results)
{
	IntersectionResult result;
	result.type = CellType::Empty;
	//result.result = HitResult::NoHit;
	if (results.Num() < 0) return result;
	double halfCellSize = c_settings.grid.cellSize / 2;

	double nearestDistance = -1;

	for (int i = 0; i < results.Num(); ++i) {
		auto res = results[i];
		auto pos = res.pos;
		FBox2D box = FBox2D(FVector2D(pos.x, pos.y) - halfCellSize, FVector2D(pos.x, pos.y) + halfCellSize);
		const FVector2D closestCellsPoint = box.GetClosestPointTo(atPoint);
		double distance = FVector2D::Distance(atPoint, closestCellsPoint);
		const float ballRadius = 0.5f;
		if ((nearestDistance == -1 || nearestDistance > distance) && distance<= ballRadius) {
			result = res;
			//UE_LOG(LogGame, Display, TEXT("getNearestIntersectionResult dirrection: %s, %f, %f"), ToString(result.dirr), xDis, yDis);
		}
	}
	
	return result;
}

void Game::UpdateDirrectionAfterHit(Breakout::IntersectionResult hit, FVector2D& dirrection)
{
	if (hit.type == CellType::DeadZone) {
		gameOver();
	}

	if (hit.type == CellType::Paddle) {
		const auto& Links = m_paddle.Get()->links();
		uint32 index = 0;

		for (const auto& Link : Links) {
			if (Link.x == (int32)(hit.pos.x + 0.5)) {
				float xDir = ((index / (float)Links.Num()) - 0.5f) * -2;
				const bool negative = xDir < 0;
				//const float absX = FMath::Clamp(FMath::Abs(xDir), 0.5, 1);
				dirrection = FVector2D(FMath::Clamp(xDir, -1, 1), -(FMath::Abs(dirrection.Y)));
		//		UE_LOG(LogGame, Display, TEXT("UpdateDirrectionAfterHit paddle: %s"), *dirrection.ToString());
				break;
			}
			++index;
		}
	}

	if (hit.type == CellType::Block || hit.type == CellType::SuperBlock || hit.type == CellType::Wall) {
		FVector2D dirrectionMultiplier;
		if (hit.type == CellType::Block || hit.type == CellType::SuperBlock) {
			onBlockHit(hit .pos);
		}
		const double xDiff = FMath::Abs(hit.hitPoint.X) - FMath::Abs(hit.ballPoint.X);
		const double xDiffAbs = FMath::Abs(xDiff);
		const double yDiff = FMath::Abs(hit.hitPoint.Y) - FMath::Abs(hit.ballPoint.Y);
		const double yDiffAbs = FMath::Abs(yDiff);

		if (xDiffAbs < yDiffAbs) {
			dirrection *= FVector2D(1, -1);
		//	UE_LOG(LogGame, Display, TEXT("UpdateVerticalDirrectionAfterHit: %f, %f, dirr: %s, with: %s, hit pos: %d, %d, ball: %s"), xDiffAbs, yDiffAbs, *dirrection.ToString(), ToString(hit.type), hit.pos.x, hit.pos.y, *hit.ballPoint.ToString());
		} else {
			dirrection *= FVector2D(-1, 1);
		//	UE_LOG(LogGame, Display, TEXT("UpdateHorizontalDirrectionAfterHit: %f, %f, dirr: %s, with %s, hit pos: %d, %d, ball: %s"), xDiffAbs, yDiffAbs, *dirrection.ToString(), ToString(hit.type), hit.pos.x, hit.pos.y, *hit.ballPoint.ToString());
		}
	}
	

}

//FVector2D Game::getDirrectionAfterHit(FVector2D pos, FVector2D oldPos, FVector2D dirrection)
//{
//	//UE_LOG(LogGame, Display, TEXT("getDirrectionAfterHit: %s, %s, %s"), ToString(m_grid->hitResult(pos)), ToString(m_grid->hitCellType(pos)), *pos.ToString());
//	FVector2D resultDirrection = dirrection;
//	switch (m_grid->hitResult(pos)) {
//		case HitResult::Changes:
//			switch (m_grid->hitCellType(pos)) {
//				case CellType::Block:
//					{
//						bool rightWall = m_grid->hitTest(Position(1, 0) + oldPos, CellType::Block);
//						bool leftWall = m_grid->hitTest(Position(-1, 0) + oldPos, CellType::Block);
//						bool ceiling = m_grid->hitTest(Position(0, -1) + oldPos, CellType::Block);
//						bool floor = m_grid->hitTest(Position(0, 1) + oldPos, CellType::Block);
//
//						resultDirrection *= FVector2D(rightWall || leftWall ? -1 : 1, ceiling || floor ? -1 : 1);
//						UE_LOG(LogGame, Display, TEXT("Changed dirrection with block from: %s, to: %s. At pos: %s"), *dirrection.ToString(), *resultDirrection.ToString(), *pos.ToString());
//						onBlockHit(pos);
//					}break;
//				case CellType::Wall:
//					{
//						bool rightWall = m_grid->hitTest(Position(1, 0) + oldPos, CellType::Wall);
//						bool leftWall = m_grid->hitTest(Position(-1, 0) + oldPos, CellType::Wall);
//						bool ceiling = m_grid->hitTest(Position(0, -1) + oldPos, CellType::Wall);
//
//						resultDirrection *= FVector2D(rightWall || leftWall ? -1 : 1, ceiling ? -1 : 1);
//						UE_LOG(LogGame, Display, TEXT("Changed dirrection with wall from: %s, to: %s. At pos: %s"), *dirrection.ToString(), *resultDirrection.ToString(), *pos.ToString());
//
//					} break;
//				case CellType::Paddle:
//					{
//						const auto& Links = m_paddle.Get()->links();
//						uint32 index = 0;
//						//	UE_LOG(LogGame, Display, TEXT("Paddle links num: %d"), Links.Num());
//
//						for (const auto& Link : Links) {
//							//		UE_LOG(LogGame, Display, TEXT("Paddle link num: %d, with index: %d"), Link.x, index);
//							if (Link.x == (int32)(pos.X + 0.5)) {
//								resultDirrection += FVector2D(((index / (float)Links.Num()) - 0.5f) * -2, 0); //Hit on a paddle changes angle
//								resultDirrection *= FVector2D(1, -1); //Hit on a paddle changes dirrection
//								UE_LOG(LogGame, Display, TEXT("-------------------- \n Changed dirrection from : % s, to : % s"), *dirrection.ToString(), *resultDirrection.ToString());
//								return resultDirrection;
//								//break;
//							}
//							++index;
//						}
//					}
//					break;
//
//
//				case CellType::DeadZone:
//					{
//						UE_LOG(LogGame, Display, TEXT("----------------GameOver!-------------------"));
//						m_gameOver = true;
//					}
//
//					break;
//				default:
//					//UE_LOG(LogGame, Display, TEXT("Temp dirrection: %f, %f"), tempDirrection.X, tempDirrection.Y);
//					break;
//			}
//
//			break;
//		case HitResult::Fault:
//			UE_LOG(LogGame, Display, TEXT("----------------GameOver!-------------------"));
//			m_gameOver = true;
//			break;
//		case HitResult::NoHit:
//
//			break;
//
//	}
//	return resultDirrection;
//}

void Game::onBlockHit(const Position& pos, bool superBlock)
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
		if (superBlock) {
			m_score += 10;
			dispatchEvent(GameplayEvent::SuperBlockBroken);
		} else {
			++m_score;
			dispatchEvent(GameplayEvent::BlockBroken);
		}
		if (m_blocks.IsEmpty()) {
			m_gameOver = true;
			dispatchEvent(GameplayEvent::GameCompleted);
		}
	}
}

void Game::updateGrid()
{
	if (!m_gridChanged) return;
	m_grid->update(m_paddle->body(), CellType::Paddle, true);
	//m_grid->update(m_ball->body(), CellType::Ball, true);
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

void Game::gameOver()
{
	m_gameOver = true;
	dispatchEvent(GameplayEvent::GameOver);
}

void Game::subscribeOnGameplayEvent(GameplayEventCallback callback)
{
	m_gameplayEventCallbacks.Add(callback);
}

void Game::dispatchEvent(GameplayEvent Event)
{
	for (const auto& callback : m_gameplayEventCallbacks) {
		if (callback) {
			callback(Event);
		}
	}
}
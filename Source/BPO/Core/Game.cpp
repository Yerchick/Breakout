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

	newPos = oldAccuratePos + m_ball->deltaAccurate() + delta;
	potentialNewPos = m_ball->getRoundedPosition(newPos);

	float ballRadius = c_settings.grid.cellSize/2; 

	auto points = getAllIntersectingResults(newPos, ballRadius);
	if (!points.IsEmpty()) {
		auto intersectionResult = getNearestIntersectionResult(newPos, points);
		UpdateDirrectionAfterHit(intersectionResult, m_ballDirrection);
	}

	

	
	//auto points = getAllIntersectingPositions(newPos, ballRadius, CellType::Wall);
	//if (!points.IsEmpty()) {
	//	potentialNewPos = getNearestIntersectionPoint(newPos, points);
	//	m_ballDirrection = getDirrectionAfterHit(newPos, oldAccuratePos, m_ballDirrection);
	//}




	/*if ((deltaSummary).Length() >= 1) {
		FVector2D tempDirrection = m_ballDirrection;
		FVector2D tempOldPos = oldAccuratePos;
		FVector2D tempdelta = delta;
		for (int i = 0; i < deltaSummary.Length(); ++i) {
			auto pos = tempOldPos + i / deltaSummary.Length() * tempdelta;
			UE_LOG(LogGame, Display, TEXT("moveBall. Scaning at index: %d"), i);
			tempDirrection = getDirrectionAfterHit(pos, tempOldPos, tempDirrection);
			tempOldPos = pos;
			tempdelta = tempDirrection * deltaSeconds * ballSpeed * m_gameSpeed;
			continue;
		}
		oldAccuratePos = tempOldPos;
		m_ballDirrection = tempDirrection;

		delta = (m_ballDirrection * deltaSeconds * ballSpeed * m_gameSpeed);
	} else {
		UE_LOG(LogGame, Display, TEXT("moveBall. Scaning normaly"));
		m_ballDirrection = getDirrectionAfterHit(newPos, oldAccuratePos, m_ballDirrection);
	}*/
	m_ball->move(potentialNewPos, newPos);
}

TArray<Breakout::IntersectionResult> Game::getAllIntersectingResults(FVector2D atPoint, float radius)
{
	TArray<Breakout::IntersectionResult> results;
	FVector2D intersectionPoint = atPoint;
	for (int i = 0; i < 4; ++i) {
		Breakout::IntersectionResult result;
		result.result = HitResult::NoHit;
		CellType type = CellType::Empty;
		switch (i) {
			case 0:
				result.dirr = Dirrection::Up;
				intersectionPoint += FVector2D(0, radius);
				break;
			case 1:
				result.dirr = Dirrection::Down;
				intersectionPoint += FVector2D(0, -radius);
				break;
			case 2:
				result.dirr = Dirrection::Right;
				intersectionPoint += FVector2D(radius, 0);
				break;
			case 3:
				result.dirr = Dirrection::Left;
				intersectionPoint += FVector2D(-radius, 0);
				break;
		}
		Breakout::Position pos = m_ball->getRoundedPosition(intersectionPoint);
	
		if (!m_grid->hitTest(pos, CellType::Wall)) {
			result.type = CellType::Wall;
			result.result = HitResult::Changes;
		}
		if (!m_grid->hitTest(pos, CellType::Block)) {
			result.type = CellType::Block;
			result.result = HitResult::Changes;
		}
		if (!m_grid->hitTest(pos, CellType::Paddle)) {
			result.type = CellType::Paddle;
			result.result = HitResult::Changes;
		}
		if (!m_grid->hitTest(pos, CellType::DeadZone)) {
			result.type = CellType::DeadZone;
			result.result = HitResult::Fault;
		}
		results.Add(result);
	}
	return results;
}

//TArray<Breakout::Position> Game::getAllIntersectingPositions(FVector2D atPoint, float radius, CellType cellType)
//{
//	TArray<Breakout::Position> result;
//	FVector2D intersectionPoint = atPoint;
//	for (int i = 0; i < 4; ++i) {
//		switch (i) {
//			case 0:
//				intersectionPoint += FVector2D(0, radius);
//				break;
//			case 1:
//				intersectionPoint += FVector2D(0, -radius);
//				break;
//			case 2:
//				intersectionPoint += FVector2D(radius, 0);
//				break;
//			case 3:
//				intersectionPoint += FVector2D(-radius, 0);
//				break;
//		}
//		Breakout::Position pos = m_ball->getRoundedPosition(intersectionPoint);
//		if (!m_grid->hitTest(pos, CellType::Wall)) {
//			result.Add(pos);
//		}
//		if (!m_grid->hitTest(pos, CellType::Block)) {
//			result.Add(pos);
//		}
//		if (!m_grid->hitTest(pos, CellType::Paddle)) {
//			result.Add(pos);
//		}
//	}
//	return result;
//}

Breakout::IntersectionResult Game::getNearestIntersectionResult(FVector2D atPoint, TArray<Breakout::IntersectionResult> results)
{
	IntersectionResult result;
	result.result = HitResult::NoHit;
	if (results.Num() < 0) return result;
	double halfCellSize = c_settings.grid.cellSize / 2;

	double nearestDistance = -1;

	for (int i = 0; i < results.Num(); ++i) {
		auto res = results[i];
		auto pos = res.pos;
		FBox2D box = FBox2D(FVector2D(pos.x, pos.y) - halfCellSize, FVector2D(pos.x, pos.y) + halfCellSize);
		double distance = FVector2D::Distance(atPoint, box.GetClosestPointTo(atPoint));
		if (nearestDistance == -1 || nearestDistance > distance) {
			nearestDistance = distance;
			result = res;
		}
	}
	return result;
}

//Breakout::Position Game::getNearestIntersectionPoint(FVector2D atPoint, TArray<Breakout::Position> positions)
//{
//	if (positions.Num() < 0) return Position{ 0,0 };
//	double nearestDistance = -1;
//	Position nearestPoint = Position{ 0,0 };
//	double halfCellSize = c_settings.grid.cellSize / 2;
//	for (int i = 0; i < positions.Num(); ++i) {
//		auto pos = positions[i];
//		FBox2D box = FBox2D(FVector2D(pos.x, pos.y) - halfCellSize, FVector2D(pos.x, pos.y) + halfCellSize);
//		double distance = FVector2D::Distance(atPoint, box.GetClosestPointTo(atPoint));
//		if (nearestDistance == -1 || nearestDistance > distance) {
//			nearestDistance = distance;
//			nearestPoint = pos;
//		}
//	}
//	return nearestPoint;
//}

void Game::UpdateDirrectionAfterHit(Breakout::IntersectionResult hit, FVector2D& dirrection)
{
	if (hit.result == HitResult::Changes) {
		FVector2D dirrectionMultiplier;
		if (hit.dirr == Dirrection::Up || hit.dirr == Dirrection::Down) {
			dirrection *= FVector2D(1, -1);
		} else if (hit.dirr == Dirrection::Right || hit.dirr == Dirrection::Left) {
			dirrection *= FVector2D(-1, 1);
		}
		if (hit.type == CellType::Paddle) {
			const auto& Links = m_paddle.Get()->links();
			uint32 index = 0;

			for (const auto& Link : Links) {
				if (Link.x == (int32)(hit.pos.x + 0.5)) {
					dirrection += FVector2D(((index / (float)Links.Num()) - 0.5f) * -2, 0); 
				}
				++index;
			}
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

// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Ball.h"
#include "Core/Game.h"

DEFINE_LOG_CATEGORY_STATIC(LogBall, All, All)

using namespace Breakout;

Ball::Ball(const Settings& settings)
{
	m_positionAccurate = FVector2D(settings.paddle.startPosition.x, settings.paddle.startPosition.y - 1);
	m_position = getRoundedPosition(m_positionAccurate);
	m_link.AddHead(m_position);
}

void Ball::correctPosition(const FVector2D delta)
{
	m_deltaAccurate += delta;
}

void Ball::move(Position newPosition, FVector2D preciseLocation)
{
	//UE_LOG(LogBall, Display, TEXT("Move ball: %d, %d, newLocation: %s"), newPosition.x, newPosition.y, *preciseLocation.ToString());
	m_position = newPosition;
	m_positionAccurate = preciseLocation;
	m_link.Empty();
	m_link.AddHead(newPosition);
	m_deltaAccurate = FVector2D(0,0);
}

const Position Ball::getRoundedPosition(FVector2D inPos)
{
	return Position((uint32)inPos.X + 0.5, (uint32)inPos.Y + 0.5);
}



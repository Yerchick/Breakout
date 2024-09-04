// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Ball.h"
#include "Core/Game.h"

using namespace Breakout;

Ball::Ball(const Settings& settings)
{
	m_positionAccurate = FVector2D( settings.paddle.startPosition.x , settings.paddle.startPosition.y-1);
	m_link.AddHead(getRoundedPosition(m_positionAccurate));
}

void Ball::correctPosition(const FVector2D delta)
{
	m_positionAccurate += delta;
}

void Ball::move(Position newPosition)
{
		m_link.Empty();
		m_link.AddHead(newPosition);
}

const Position Ball::getRoundedPosition(FVector2D inPos)
{
	return Position((uint32)inPos.X + 0.5 , (uint32)inPos.Y + 0.5);
}



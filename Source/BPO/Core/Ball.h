// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h"

namespace Breakout
{
class  Ball
{

public:
	Ball(const Settings& settings);
	void move(Position newPosition, FVector2D preciseLocation);
	void correctPosition(const FVector2D delta);
	const FVector2D positionAccurate() const { return m_positionAccurate; };
	const FVector2D deltaAccurate() const { return m_deltaAccurate; };
	const TPositionPtr* body() const { return m_link.GetHead(); }
	const Position getPosition() const { return m_position; }

	const Position getRoundedPosition(FVector2D inPos);

private:
	TPositionList m_link;
	Position m_position {0,0};
	FVector2D m_positionAccurate{ 0,0 };
	FVector2D m_deltaAccurate{ 0,0 };

};
} //namespace Breakout
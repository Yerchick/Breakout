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
	void move(Position newPosition);
	void correctPosition(const FVector2D delta);
	const FVector2D positionAccurate() const { return m_positionAccurate; };
	const TPositionPtr* body() const { return m_link.GetHead(); }

	const Position getRoundedPosition(FVector2D inPos);

private:
	TPositionList m_link;
	FVector2d m_positionAccurate{ 0,0 };

};
} //namespace Breakout
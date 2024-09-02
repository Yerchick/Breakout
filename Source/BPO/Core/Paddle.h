// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h"

namespace Breakout
{
class  Paddle
{
public:
	Paddle(const Settings& settings);

	void move(const Input& input);
	
	const TPositionPtr* body() const { return m_links.GetHead()->GetNextNode(); }

private:
	TPaddleList m_links;

};

} //namespace Breakout
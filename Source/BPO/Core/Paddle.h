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
};
} //namespace Breakout
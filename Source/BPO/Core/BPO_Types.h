// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"

namespace Breakout {

	struct Dim {
		uint32 width;
		uint32 height;
	};

	enum class CellType {
		Empty = 0,
		Wall = 1,
		Block = 2,
		SuperBlock = 3,
		PaddleZone = 4,
		DeadZone = 5
	};

	struct Settings {
		Dim gridSize;
	};

}
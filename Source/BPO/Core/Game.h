// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h" 

namespace Breakout
{

class Grid;

class Game
{
	public:
	Game(const Settings& settings);
	TSharedPtr<Grid> grid() const { return m_grid; };

	private: 
	const Settings c_settings;
	TSharedPtr<Grid> m_grid;
};
} //namespace Breakout


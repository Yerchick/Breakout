// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Block.h"

using namespace Breakout;

void Breakout::Block::setPossitions(const TArray<Breakout::Position>& positions) 
{
	m_positions = positions;
}

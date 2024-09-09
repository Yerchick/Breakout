// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h"

namespace Breakout
{



class Block
{
public:
	Block() = default;

	void setPossitions(const TArray<Breakout::Position>& positions);
	void setType(BlockType inType);
	TArray<Breakout::Position> positions() const { return m_positions; };
	BlockType type() const { return m_type; }

private:

	BlockType m_type;
	TArray<Breakout::Position> m_positions;
};

} //namespace Breakout
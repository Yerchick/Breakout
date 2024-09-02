// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h"

namespace Breakout {

	class  Grid
	{
	public:
		Grid(const Breakout::Settings& settings);

		Dim dim() const { return c_dim; }
		void update(const TPositionPtr* links, CellType type);
		void printDebug();

	private:
		const uint8 deadZoneHeight;
		const uint8 paddleZoneHeight;
		const uint8 wallWidth;
		const Dim c_dim;
		TArray<CellType> m_cells;

		void initWalls();

		

		FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
		FORCEINLINE uint32 posToIndex(Position pos) const;
	};


}


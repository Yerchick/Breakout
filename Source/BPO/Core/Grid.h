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


	private:
		const Dim c_dim;
		TArray<CellType> m_cells;

		void initWalls();
		void printDebug();
		FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
	};


}


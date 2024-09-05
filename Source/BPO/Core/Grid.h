// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BPO_Types.h"

namespace Breakout {

class Block;

	class  Grid
	{
	public:
		Grid(const Breakout::Settings& settings);

		Dim dim() const { return c_dim; }
		void update(const TPositionPtr* links, CellType type, bool debug = false);
		void update(const TArray<TSharedPtr<Block>> blocks, CellType cellType);
		void update(const Position& pos, CellType cellType);
		bool hitTest(const Position& pos, CellType cellType) const;
		HitResult hitResult(const Position& pos) const;
		CellType hitCellType(const Position& pos) const;
		FUintRect getFreeSpace() const { return freeSpace; }
		void printDebug();

	private:

		FUintRect freeSpace;
		const uint8 deadZoneHeight;
		const uint8 paddleZoneHeight;
		const uint8 wallWidth;
		const Dim c_dim;
		TArray<CellType> m_cells;

		void initWalls();

		FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
		FORCEINLINE uint32 posToIndex(const Position& pos) const;
		void freeCellsByType(CellType type);
	};


}


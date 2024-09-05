// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Grid.h"
#include "Core/Block.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All)

using namespace Breakout;


Grid::Grid(const Breakout::Settings& settings) // Leave a space for walls and a field for a paddle and deadzone
	: c_dim(Dim{
			settings.grid.gridSize.width + settings.grid.wallWidth * 2 ,
			settings.grid.gridSize.height + settings.grid.deadzoneHeight + settings.paddle.height + settings.grid.wallWidth
		}),
	deadZoneHeight(settings.grid.deadzoneHeight),
	paddleZoneHeight(settings.paddle.height),
	wallWidth(settings.grid.wallWidth)
{

	m_cells.Init(CellType::Empty, c_dim.width * c_dim.height);
	initWalls();
	//printDebug();
}



void Grid::initWalls()
{
	freeSpace = {99999,99999,0,0};
	for (uint32 y = 0; y < c_dim.height; ++y) {
		for (uint32 x = 0; x < c_dim.width; ++x) {
			if (x < wallWidth || x >= c_dim.width - wallWidth || y < wallWidth) {
				m_cells[posToIndex(x, y)] = CellType::Wall;
			} else if (y >= c_dim.height - (paddleZoneHeight + deadZoneHeight) && y < c_dim.height - deadZoneHeight) {
				m_cells[posToIndex(x, y)] = CellType::PaddleZone;
			} else if (y >= c_dim.height - deadZoneHeight) {
				m_cells[posToIndex(x, y)] = CellType::DeadZone;
			} else {
				//empty free space 
				if (freeSpace.Min.X > x) {
					freeSpace.Min.X = x;
				}else if(freeSpace.Min.Y > y) {
					freeSpace.Min.Y = y;
				} else if (freeSpace.Max.X <= x) {
					freeSpace.Max.X = x;
				} else if (freeSpace.Max.Y <= y) {
					freeSpace.Max.Y = y;
				}
			}
		}
	}
	UE_LOG(LogGrid, Display, TEXT("initWalls Finished. FreeSpace: %s"), *freeSpace.ToString());
}



void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
	UE_LOG(LogGrid, Display, TEXT("printDebug"));
	for (uint32 y = 0; y < c_dim.height; ++y) {
		FString line;
		for (uint32 x = 0; x < c_dim.width; ++x) {
			TCHAR symbol;
			switch (m_cells[posToIndex(x, y)]) {

				case CellType::Wall: symbol = 'w'; break;
				case CellType::PaddleZone: symbol = '_'; break;
				case CellType::DeadZone: symbol = 'x'; break;
				case CellType::Paddle: symbol = '='; break;
				case CellType::Ball: symbol = 'o'; break;
				case CellType::Block: symbol = 'b'; break;
				case CellType::SuperBlock: symbol = 's'; break;
				default:
				case CellType::Empty: symbol = ' '; break;
					break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
#endif
}

void Grid::update(const TPositionPtr* links, CellType type, bool debug)
{
	freeCellsByType(type);
	auto* link = links;
	while (link) {
		const auto index = posToIndex(link->GetValue());
		if (m_cells.IsValidIndex(index)) {
			m_cells[index] = type;
			if (debug) {
				//UE_LOG(LogGrid, Display, TEXT("Update: %s, index: %d, value: %d"), ToString(type), index, link->GetValue().x);
			}
		}
		link = link->GetNextNode();
	}
}

void Grid::update(const TArray<TSharedPtr<Block>> blocks, CellType cellType)
{
	for (int i = 0; i < blocks.Num(); ++i) {
		auto position = blocks[i].Get()->positions()[0];
		const auto index = posToIndex(position);
		//UE_LOG(LogGrid, Display, TEXT("Update blocks: %s, index: %d, value: %d, %d"), ToString(cellType), index, position.x, position.y);
		m_cells[index] = cellType;
	}
}

void Grid::update(const Position& pos, CellType cellType)
{
		const auto index = posToIndex(pos);
		//UE_LOG(LogGrid, Display, TEXT("Update blocks: %s, index: %d, value: %d, %d"), ToString(cellType), index, position.x, position.y);
		m_cells[index] = cellType;
}

void Grid::freeCellsByType(CellType type)
{
	////TODO optimisation
	for (uint32 y = 0; y < c_dim.height; ++y) {
		for (uint32 x = 0; x < c_dim.width; ++x) {
			if (m_cells[posToIndex(x, y)] == type) {

				if /*(x < wallWidth || x >= c_dim.width - wallWidth || y < wallWidth) {
					m_cells[posToIndex(x, y)] = CellType::Wall;
				} else if*/ (y >= c_dim.height - (paddleZoneHeight + deadZoneHeight) && y < c_dim.height - deadZoneHeight) {
					m_cells[posToIndex(x, y)] = CellType::PaddleZone;
				} else if (y >= c_dim.height - deadZoneHeight) {
					m_cells[posToIndex(x, y)] = CellType::DeadZone;
				} else {
						m_cells[posToIndex(x, y)] = CellType::Empty;
				}
			}
		}
	}
}

bool Grid::hitTest(const Position& pos, CellType cellType) const
{
	int32 index = posToIndex(pos);
	if (index >= m_cells.Num() || !m_cells.IsValidIndex(index)) {
		//UE_LOG(LogGrid, Error, TEXT("HitTest Triest to use wron index: %d, %d for cell type: %s"), pos.x, pos.y, ToString(cellType));
		return true;
	}
	//UE_LOG(LogGrid, Display, TEXT("hit Test: %d, %d with cell type: %s"), pos.x, pos.y, ToString(m_cells[posToIndex(pos)]));
	return m_cells[posToIndex(pos)] == cellType;
}

CellType Grid::hitCellType(const Position& pos) const
{
	int32 index = posToIndex(pos);
	if (index >= m_cells.Num() || !m_cells.IsValidIndex(index)) {
		//UE_LOG(LogGrid, Error, TEXT("HitTest Triest to use wron index: %d, %d for cell type: %s"), pos.x, pos.y, ToString(cellType));
		return CellType::Error;
	}
	//UE_LOG(LogGrid, Display, TEXT("hit Test: %d, %d with cell type: %s"), pos.x, pos.y, ToString(m_cells[posToIndex(pos)]));
	return m_cells[posToIndex(pos)];
}

HitResult Grid::hitResult(const Position& pos) const
{
	int32 index = posToIndex(pos);
	if (index >= m_cells.Num() || !m_cells.IsValidIndex(index)) {
		return HitResult::Fault;
	} else {
		switch (m_cells[index]) {
			case CellType::Wall: return HitResult::Changes;
			case CellType::PaddleZone: return HitResult::Success;
			case CellType::DeadZone: return HitResult::Fault;
			case CellType::Paddle:  return HitResult::Changes;
			case CellType::Block:  return HitResult::Changes;
			case CellType::SuperBlock:  return HitResult::Changes;
			case CellType::Ball: return HitResult::Changes;
			default:
			case CellType::Empty: return HitResult::Success;
				break;
		}
	}
}



uint32 Grid::posToIndex(const Position& pos) const
{
	return posToIndex(pos.x, pos.y);
}

uint32 Grid::posToIndex(uint32 x, uint32 y) const
{
	return x + y * c_dim.width;
}


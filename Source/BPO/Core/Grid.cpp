// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Core/Grid.h"


DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All)

using namespace Breakout;


Grid::Grid(const Breakout::Settings& settings) // Leave a space for walls and a field for a paddle and deadzone
	: c_dim(Dim{
		settings.grid.gridSize.width + settings.grid.wallWidth * 2 ,
		settings.grid.gridSize.height + settings.grid.deadzoneHeight + settings.paddle.height + settings.grid.wallWidth
		})
{
	m_cells.Init(CellType::Empty, c_dim.width * c_dim.height);
	initWalls();
	printDebug();
}


void Grid::initWalls()
{
	for (uint32 y = 0; y < c_dim.height; ++y) {
		for (uint32 x = 0; x < c_dim.width; ++x) {
			if (x == 0 || x == c_dim.width - 1 || y == 0) {
				m_cells[posToIndex(x, y)] = CellType::Wall;
			} else if (y == c_dim.height - 2) {
				m_cells[posToIndex(x, y)] = CellType::PaddleZone;
			} else if (y == c_dim.height - 1) {
				m_cells[posToIndex(x, y)] = CellType::DeadZone;
			}
		}
	}
}

void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
	for (uint32 y = 0; y < c_dim.height; ++y) {
		FString line;
		for (uint32 x = 0; x < c_dim.width; ++x) {
			TCHAR symbol;
			switch (m_cells[posToIndex(x, y)]) {

				case CellType::Wall: symbol = '*'; break;
				case CellType::PaddleZone: symbol = '-'; break;
				case CellType::DeadZone: symbol = 'x'; break;
				default:
				case CellType::Empty: symbol = '0'; break;
					break;
			}
			line.AppendChar(symbol).AppendChar(' ');
		}
		UE_LOG(LogGrid, Display, TEXT("%s"), *line);
	}
#endif
}

uint32 Grid::posToIndex(uint32 x, uint32 y) const
{
	return x + y * c_dim.width;

}


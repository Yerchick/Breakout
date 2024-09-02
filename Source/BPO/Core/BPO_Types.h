// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"

namespace Breakout
{

struct Dim
{
	uint32 width;
	uint32 height;
};

enum class CellType
{
	Empty = 0,
	Wall = 1,
	Block = 2,
	SuperBlock = 3,
	PaddleZone = 4,
	Paddle = 5,
	DeadZone = 6,
	Ball = 7
};

struct Position
{
	Position(uint32 inX, uint32 inY) : x(inX), y(inY) {}
	uint32 x;
	uint32 y;
};





struct Settings
{
	uint8 difficulty = 1;

	//	Grid grid { Dim{30, 50}, uint8(2), uint8(2)};

	//	Paddle paddle {uint8( 4), uint8(1), uint8(100), Position{1,10} };

	struct Grid
	{
		//Grid(Dim inGridSize, uint8 inWallWidth, uint8 inDeadzoneHeight) : gridSize(inGridSize), wallWidth(inWallWidth), deadzoneHeight(inDeadzoneHeight) {}
		Dim gridSize{ 5, 10 };
		uint8 wallWidth{ 2 };
		uint8 deadzoneHeight{ 1 };
	} grid;

	struct PaddleParam
	{
		//Paddle(uint32 inWidth, uint32 inHeight, uint32 inSpeed, Position inStartPosition) : width(inWidth), height(inHeight), speed(inSpeed), startPosition(inStartPosition) {}
		uint8 width{4};
		uint8 height{1};
		uint8 speed{ 10 };
		Position startPosition{ 0,0 };

	} paddle;
};

struct Input
{
	int8 x;
};

using TPaddleList = TDoubleLinkedList<Position>;
using TPositionPtr = TPaddleList::TDoubleLinkedListNode;

} //namespace breakout
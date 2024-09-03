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

inline const TCHAR* ToString(Breakout::CellType Type)
{
	switch (Type) {
		case CellType::Empty:
			return TEXT("Empty");
		case CellType::Wall:
			return TEXT("Wall");
		case CellType::Block:
			return TEXT("Block");
		case CellType::SuperBlock:
			return TEXT("SuperBlock");
		case CellType::PaddleZone:
			return TEXT("PaddleZone");
		case CellType::Paddle:
			return TEXT("Paddle");
		case CellType::DeadZone:
			return TEXT("DeadZone");
		case CellType::Ball:
			return TEXT("Ball");
	}
	return TEXT("Unknown");
}

struct Position
{
	Position(uint32 inX, uint32 inY) : x(inX), y(inY) {}
	uint32 x;
	uint32 y;

	FORCEINLINE Position& operator +=(const Position& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	FORCEINLINE Position& operator+(const Position& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
};





struct Settings
{
	uint8 difficulty = 1;
	float gameSpeed = 1.0f;

	struct Grid
	{
		Dim gridSize{ 5, 10 };
		uint8 wallWidth{ 2 };
		uint8 deadzoneHeight{ 1 };
	} grid;

	struct PaddleParam
	{
		uint8 width{4};
		uint8 height{1};
		uint8 speed{ 10 };
		Position startPosition{ 0,0 };
	} paddle;
};

struct Input
{
	float x;
};

using TPaddleList = TDoubleLinkedList<Position>;
using TPositionPtr = TPaddleList::TDoubleLinkedListNode;

} //namespace breakout
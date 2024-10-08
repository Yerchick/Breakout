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

enum class BlockType
{
	Normal = 0,
	Super
};

enum class CellType
{
	Empty = 0,
	Wall,
	Block,
	PaddleZone,
	Paddle,
	DeadZone,
	Ball,
	Error
};

enum class Dirrection
{
	Right = 0,
	Left = 1,
	Up = 2,
	Down = 3
};

enum class GameplayEvent
{
	GameOver = 0,
	GameCompleted,
	BlockBroken,
	SuperBlockBroken
};

using GameplayEventCallback = TFunction<void(GameplayEvent)>;

inline const TCHAR* ToString(Breakout::Dirrection dir)
{
	switch (dir) {
		case Dirrection::Right:
			return TEXT("Right");
		case Dirrection::Left:
			return TEXT("Left");
		case Dirrection::Up:
			return TEXT("Up");
		case Dirrection::Down:
			return TEXT("Down");
		default:

			return TEXT("Unknown");
	}
}

inline const TCHAR* ToString(Breakout::CellType Type)
{
	switch (Type) {
		case CellType::Empty:
			return TEXT("Empty");
		case CellType::Wall:
			return TEXT("Wall");
		case CellType::Block:
			return TEXT("Block");
		case CellType::PaddleZone:
			return TEXT("PaddleZone");
		case CellType::Paddle:
			return TEXT("Paddle");
		case CellType::DeadZone:
			return TEXT("DeadZone");
		case CellType::Ball:
			return TEXT("Ball");
		case CellType::Error:
			return TEXT("Error");
	}
	return TEXT("Unknown");
}

struct Position
{
	Position(uint32 inX, uint32 inY) : x(inX), y(inY) {}
	Position(FVector2D vector) : x(vector.X+0.5), y(vector.Y+0.5) {}
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

	FORCEINLINE bool operator==(const Position& rhs)
	{
		bool xEqual = (x == rhs.x);
		bool yEqual = (y == rhs.y);
		return (xEqual && yEqual);
	}

	FORCEINLINE bool operator!=(const Position& rhs)
	{
		bool xEqual = (x != rhs.x);
		bool yEqual = (y != rhs.y);
		return (xEqual || yEqual);
	}
};


struct Settings
{
	uint8 difficulty = 1;
	float gameSpeed = 1.0f;
	float ballSpeed = 10;

	struct Grid
	{
		Dim gridSize{ 5, 10 };
		float cellSize{ 10 };
		uint8 wallWidth{ 2 };
		uint8 deadzoneHeight{ 1 };
		FUintPoint blockSize{ 2, 1 };
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

struct IntersectionResult
{
//	HitResult result{ HitResult::Fault };
	FVector2D ballPoint{ 0,0 };
	FVector2D hitPoint{ 0,0 };
	Position pos{ 0,0 };
	CellType type{CellType::Error};
	Dirrection dirr{ Dirrection::Right };
	FVector2D dir{0,0};
};

using TPositionList = TDoubleLinkedList<Position>;
using TPositionPtr = TPositionList::TDoubleLinkedListNode;

} //namespace breakout
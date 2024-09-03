// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "World/BPO_Paddle.h"

namespace
{
FVector LinkPositionToVector(const Breakout::Position& Position, FUintPoint CellSize, const Breakout::Dim& Dims)
{
	return FVector({ (Dims.height -1 -(double)Position.y - 1) * (double)CellSize.Y, (double)Position.x * (double)CellSize.Y, 0.0f}) + FVector(CellSize)*0.5f;
}
} //namespace

ABPO_Paddle::ABPO_Paddle()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABPO_Paddle::SetModel(const TSharedPtr<Breakout::Paddle>& InPaddle, FUintPoint InCellSize, FUintPoint InPaddlelSize, const Breakout::Dim& InDims)
{
	Paddle = InPaddle;
	CellSize = InCellSize;
	PaddleSize = InPaddlelSize;
	Dims = InDims;
}


void ABPO_Paddle::BeginPlay()
{
	Super::BeginPlay();

	if (!Paddle.IsValid() || !GetWorld()) return;

	const auto& Links = Paddle.Pin()->links();

	uint32 i = 0;
	for (const auto& Link : Links) {
		const FTransform Transform = FTransform(LinkPositionToVector(Link, CellSize, Dims));
		auto* LinkActor = GetWorld()->SpawnActor<AActor>(PaddleClass, Transform);
		PaddleLinks.Add(LinkActor);
		++i;
	}
}

void ABPO_Paddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Paddle.IsValid()) return;

	const auto& Links = Paddle.Pin()->links();
	auto* LinkPtr = Links.GetHead();

	for (auto* LinkActor : PaddleLinks) {
		LinkActor->SetActorLocation(LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dims));
		LinkPtr = LinkPtr->GetNextNode();
	}
}


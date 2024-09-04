// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "World/BPO_Ball.h"


namespace
{
FVector LinkPositionToVector(const Breakout::Position& Position, FUintPoint CellSize, const Breakout::Dim& Dims)
{
	return FVector({ (Dims.height - 1 - (double)Position.y - 1) * (double)CellSize.Y, (double)Position.x * (double)CellSize.Y, 0.0f }) + FVector(CellSize) * 0.5f;
}

FVector LinkPositionToVector(const FVector2D Position, FUintPoint CellSize, const Breakout::Dim& Dims)
{
	return FVector({ (Dims.height - 1 - Position.Y - 1) * (double)CellSize.Y, Position.X * (double)CellSize.Y, 0.0f }) + FVector(CellSize) * 0.5f;
}
} //namespace

ABPO_Ball::ABPO_Ball()
{
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>("BallMesh");
	check(BallMesh);
	BallMesh->SetupAttachment(Origin);
}

void ABPO_Ball::SetModel(const TSharedPtr<Breakout::Ball>& InBall, uint32 InCellSize, const Breakout::Dim& InDims)
{
	Ball = InBall;
	CellSize = InCellSize;
	Dims = InDims;
}

void ABPO_Ball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Ball.IsValid()) return;
	
	//const auto& Link = Ball.Pin().Get()->body()->GetValue();
	//const FTransform Transform = FTransform(LinkPositionToVector(Link, CellSize, Dims));
	const FTransform Transform = FTransform(LinkPositionToVector(Ball.Pin().Get()->positionAccurate(), CellSize, Dims));
	SetActorLocation(Transform.GetLocation());
}

void ABPO_Ball::BeginPlay()
{
	Super::BeginPlay();

	if (!Ball.IsValid() || !GetWorld()) return;

	const auto& Link = Ball.Pin().Get()->body()->GetValue();

	const FTransform Transform = FTransform(LinkPositionToVector(Link, CellSize, Dims));

	check(BallMesh->GetStaticMesh());
	const FBox Box = BallMesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	check(Size.X);
	check(Size.Y);
	BallMesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize / Size.Y, CellSize / Size.Z));
	SetActorLocation(Transform.GetLocation());
		//auto* LinkActor = GetWorld()->SpawnActorDeferred<>(BallClass, Transform);
		//LinkActor->UpdateScale(CellSize, PaddleSize);
		//LinkActor->FinishSpawning(Transform);
		//BallLink.Add(LinkActor);
}






// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "World/BPO_Block.h"

namespace Breakout
{
FVector LinkPositionToVector(const Breakout::Position Position, FUintPoint CellSize, const Breakout::Dim& Dims)
{
	return FVector({ (Dims.height - 1 - (double)Position.y - 1) * (double)CellSize.Y, (double)Position.x * (double)CellSize.Y, 0.0f }) + FVector(CellSize) * 0.5f;
}

}


ABPO_Block::ABPO_Block()
{
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockMesh");
	check(BlockMesh);
	BlockMesh->SetupAttachment(Origin);
}

void ABPO_Block::SetModel(const TSharedPtr<Breakout::Block>& inBlock, uint32 inCellSize, FUintPoint inBlockSize, Breakout::Dim inDims)
{
	Block = inBlock;
	CellSize = inCellSize;
	BlockSize = inBlockSize;
	Dims = inDims;
}

void ABPO_Block::BeginPlay()
{
	Super::BeginPlay();

	if (!GetWorld()) return;

	if (!Block.IsValid()) {
		Destroy();
	}

	const auto& Links = Block.Pin()->positions();
	const auto Pos = Links[0];

	const FTransform Transform = FTransform(Breakout::LinkPositionToVector(Pos, CellSize, Dims));

	check(BlockMesh->GetStaticMesh());
	const FBox Box = BlockMesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	check(Size.X);
	check(Size.Y);
	BlockMesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize / Size.Y, CellSize / Size.Z));
	SetActorLocation(Transform.GetLocation());
}

// Called every frame
void ABPO_Block::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Block.IsValid()) {
		Block.Reset();
		Destroy();
		return;
	}

	const auto& Links = Block.Pin()->positions();
	const auto Pos = Links[0];

	const FTransform Transform = FTransform(LinkPositionToVector(Pos, CellSize, Dims));
	SetActorLocation(Transform.GetLocation());
}


// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "World/BPO_PaddleLink.h"
#include "Components/StaticMeshComponent.h"

ABPO_PaddleLink::ABPO_PaddleLink()
{
	PrimaryActorTick.bCanEverTick = false;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	LinkMesh = CreateDefaultSubobject<UStaticMeshComponent>("LinkMesh");
	check(LinkMesh);
	LinkMesh->SetupAttachment(Origin);
}

void ABPO_PaddleLink::UpdateScale(uint32 CellSize, FUintPoint PaddleSize)
{
	check(LinkMesh->GetStaticMesh());
	const FBox Box = LinkMesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	check(Size.X);
	check(Size.Y);
	LinkMesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize * PaddleSize.Y / Size.Y, CellSize / Size.Z));
}


// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "World/BPO_Grid.h"
#include "Core/Grid.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"
#include "Components/StaticMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All)

ABPO_Grid::ABPO_Grid()
{
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	GridMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridMesh");
	check(GridMesh);
	GridMesh->SetupAttachment(Origin);
}

void ABPO_Grid::BeginPlay()
{
	Super::BeginPlay();
	
}


void ABPO_Grid::SetModel(const TSharedPtr<Breakout::Grid>& Grid, FUintPoint InCellSize)
{
	if (!Grid.IsValid()) {
		UE_LOG(LogWorldGrid, Fatal, TEXT("Grid is null, game aborted!"));
	}
	GridDim = Grid.Get()->dim();
	CellSize = InCellSize;
	WorldSize = FUintPoint((CellSize.X * GridDim.width), (CellSize.Y * GridDim.height));

	UE_LOG(LogWorldGrid, Display, TEXT("Grid size: %d, %d, Cell Size: %d, %d, World Size: %d, %d"), GridDim.width, GridDim.height, CellSize.X, CellSize.Y, WorldSize.X, WorldSize.Y);

	//scale mesh
	check(GridMesh->GetStaticMesh());
	const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
	const auto Size = Box.GetSize();

	GridMesh->SetRelativeScale3D(FVector(WorldSize.Y / Size.X, WorldSize.X / Size.Y, 1.0));
	GridMesh->SetRelativeLocation(0.5 * FVector(WorldSize.Y, WorldSize.X, -Size.Z));

	//setup material
	GridMaterial = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (GridMaterial) {
		GridMaterial->SetVectorParameterValue("Division", FVector(GridDim.height, GridDim.width, 0.0));
	}

}

void ABPO_Grid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//	DrawGrid();
}

void ABPO_Grid::DrawGrid()
{
	for (uint32 i = 0; i < GridDim.height + 1; ++i) {
		const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * CellSize.Y * i;
		const FVector EndLocation = StartLocation + GetActorRightVector() * WorldSize.X;
		//DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorRightVector() * WorldWidth, FColor::Red, false, -1.0f);
		GetWorld()->LineBatcher->DrawLine( StartLocation, EndLocation, FColor::Red, 1, -1.0f);
	}
	for (uint32 i = 0; i < GridDim.width + 1; ++i) {
		const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize.X * i;
		const FVector EndLocation = StartLocation + GetActorForwardVector() * WorldSize.Y;
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f);
		GetWorld()->LineBatcher->DrawLine(StartLocation, EndLocation, FColor::Green, 1, -1.0f);
	}
}


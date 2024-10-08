// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/BPO_Types.h"
#include "BPO_Grid.generated.h"

namespace Breakout
{
class Grid;
}

class UStaticMeshComponent;

UCLASS()
class ABPO_Grid : public AActor
{
	GENERATED_BODY()

public:
	ABPO_Grid();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* GridMesh;

public:
	virtual void Tick(float DeltaTime) override;
	void SetModel(const TSharedPtr<Breakout::Grid>& Grid, uint32 InCellSize, uint8 InWallThickness);

private:
	UPROPERTY()
	UMaterialInstanceDynamic* GridMaterial;

	Breakout::Dim GridDim;
	uint32 CellSize;
	FUintPoint WorldSize;
	uint8 WallThickness;

	void DrawGrid();
};




// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Block.h"
#include "BPO_Block.generated.h"



class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class ABPO_Block : public AActor
{
	GENERATED_BODY()
	
public:	
	ABPO_Block();

	void SetModel(const TSharedPtr<Breakout::Block>& inBlock, uint32 inCellSize, FUintPoint inBlockSize, Breakout::Dim inDims);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BlockMesh;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	TWeakPtr<Breakout::Block> Block;
	uint32 CellSize;
	FUintPoint BlockSize;
	Breakout::Dim Dims;
};

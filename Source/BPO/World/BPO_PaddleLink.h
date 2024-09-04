// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPO_PaddleLink.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class BPO_API ABPO_PaddleLink : public AActor
{
	GENERATED_BODY()

public:
	ABPO_PaddleLink();

	void UpdateScale(uint32 CellSize, FUintPoint PaddleSize);

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LinkMesh;
};

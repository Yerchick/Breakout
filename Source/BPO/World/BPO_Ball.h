// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Ball.h"
#include "BPO_Ball.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class BPO_API ABPO_Ball : public AActor
{
	GENERATED_BODY()
	
public:	
	ABPO_Ball();

	void SetModel(const TSharedPtr<Breakout::Ball>& InBall, uint32 InCellSize, const Breakout::Dim& InDims);

	virtual void Tick(float DeltaTime) override;
	virtual void  BeginPlay() override;

private:
	TWeakPtr<Breakout::Ball> Ball;
	uint32 CellSize;
	Breakout::Dim Dims;

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BallMesh;
};

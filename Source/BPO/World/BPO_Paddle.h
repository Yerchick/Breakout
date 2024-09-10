// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BPO/Core/Paddle.h"
#include "BPO_Paddle.generated.h"


class ABPO_PaddleLink;

UCLASS()
class BPO_API ABPO_Paddle : public AActor
{
	GENERATED_BODY()
	
public:	
	ABPO_Paddle();

	void SetModel(const TSharedPtr<Breakout::Paddle>& InPaddle, uint32 InCellSize, FUintPoint InPaddlelSize, const Breakout::Dim& InDims);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABPO_PaddleLink> PaddleClass;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	TWeakPtr<Breakout::Paddle> Paddle;
	uint32 CellSize;
	FUintPoint PaddleSize;
	Breakout::Dim Dims;

	UPROPERTY()
	TArray<AActor*> PaddleLinks;

	FVector LinkPositionToVector(const Breakout::Position& Position);
};

// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/BPO_Types.h"
#include "BPO_Pawn.generated.h"

class UCameraComponent;

UCLASS()
class ABPO_Pawn : public APawn
{
	GENERATED_BODY()

public:
	ABPO_Pawn();

	void UpdateLocation(const Breakout::Dim& InDim, const FUintPoint InCellSize, const FTransform& InGridOrigin);

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Origin;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

private:
	Breakout::Dim Dim;
	FUintPoint CellSize;
	FTransform GridOrigin;

	void OnViewportResized(FViewport* Viewport, uint32 Val);
	FDelegateHandle ResizeHandle;
};

// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Framework/BPO_Pawn.h"
#include "Camera/CameraComponent.h"

namespace
{
float FOVTan(float FOVDegrees)
{
	return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5));
}
float VerticalFOV(float HorizontalFOVDegrees, float AspectRatio)
{
	return FMath::RadiansToDegrees(2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorizontalFOVDegrees * 0.5) * AspectRatio)));
}
}


// Sets default values
ABPO_Pawn::ABPO_Pawn()
{
	PrimaryActorTick.bCanEverTick = false;
	Origin = CreateDefaultSubobject<USceneComponent>("Origin");
	check(Origin);
	SetRootComponent(Origin);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	check(Camera);
	Camera->SetupAttachment(Origin);
}

void ABPO_Pawn::UpdateLocation(const Breakout::Dim& InDim, const FUintPoint InCellSize, const FTransform& InGridOrigin)
{
	Dim = InDim;
	CellSize = InCellSize;
	GridOrigin = InGridOrigin;

	check(GEngine);
	check(GEngine->GameViewport);
	check(GEngine->GameViewport->Viewport);

	auto* Viewport = GEngine->GameViewport->Viewport;
	Viewport->ViewportResizedEvent.Remove(ResizeHandle);
	ResizeHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ABPO_Pawn::OnViewportResized);

#if WITH_EDITOR
	OnViewportResized(Viewport, 0);
#endif
}

void ABPO_Pawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
	if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.height == 0) {
		return;
	}

	const FUintPoint WorldSize = FUintPoint{ Dim.width * CellSize.X, Dim.height * CellSize.Y };
	const double WorldHeight = Dim.height * CellSize.Y;
	const double WorldWidth = Dim.width * CellSize.X;

	const double ViewHeight = Viewport->GetSizeXY().Y;
	const double ViewWidth = Viewport->GetSizeXY().X;


	const double AspectRatio = 1 / (ViewHeight / ViewWidth);
	const double GridAspect = static_cast<double>(WorldWidth) / WorldHeight;


	double LocationZ = 0.0;
	
	if (AspectRatio < GridAspect) {
		//UE_LOG(LogTemp, Display, TEXT("Horizontal aspect is: %f"), GridAspect);
		//LocationZ =  (WorldWidth)  / FOVTan(Camera->FieldOfView);
		//LocationZ = (WorldSize.X * 0.5)  / FMath::Tan(FMath::DegreesToRadians((Camera->FieldOfView * 0.5) * AspectRatio));

		//float tan = FMath::Tan(FMath::DegreesToRadians((Camera->FieldOfView * 0.5)));
		float tan = FOVTan(Camera->FieldOfView);
		LocationZ = (WorldWidth * ViewHeight) / ( tan * ViewWidth);
		
	
		UE_LOG(LogTemp, Display, TEXT("Horizontal aspect is: %f, AspectRatio: %f, LocationZ: %f, WorldSize: %d "), GridAspect, AspectRatio, LocationZ, WorldSize.X );
	} else {
		
		check(AspectRatio);
		const float  tan = FOVTan(Camera->FieldOfView);
		const float FOV = VerticalFOV(Camera->FieldOfView, ViewHeight / ViewWidth);
		const float FovTan = FOVTan(FOV);
		LocationZ = (WorldHeight* WorldHeight) / (tan * tan * ViewHeight*0.5);
		//float tan = FMath::Tan(FMath::DegreesToRadians((Camera->FieldOfView * 0.5)));
		//LocationZ = (WorldHeight * ViewHeight) / (tan * ViewWidth);

		UE_LOG(LogTemp, Display, TEXT("Vertical aspect is: %f, AspectRatio: %f, LocationZ: %f, WorldSize: %d, fovTan: %f, viewH: %f "), GridAspect, AspectRatio, LocationZ, WorldSize.Y, FovTan, ViewHeight);
	}
	
	const FVector NewPawnLocation = GridOrigin.GetLocation() +  FVector(WorldHeight * 0.5, WorldWidth * 0.5, LocationZ);
	SetActorLocation(NewPawnLocation);
}





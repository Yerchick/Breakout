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
	return FMath::RadiansToDegrees(2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorizontalFOVDegrees) * 0.5) * AspectRatio));
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


	const double AspectRatio = 1 / (static_cast<double>((ViewWidth) / (ViewHeight)));
	const double GridAspect = static_cast<double>(WorldWidth) / WorldHeight;


	double LocationZ = 0.0;
	
	if (AspectRatio >= GridAspect) {
		//UE_LOG(LogTemp, Display, TEXT("Horizontal aspect is: %f"), GridAspect);
		LocationZ = (WorldSize.X * 0.5)  / FOVTan(Camera->FieldOfView);
		//LocationZ = (WorldSize.X * 0.5)  / FMath::Tan(FMath::DegreesToRadians((Camera->FieldOfView * 0.5) * AspectRatio));
	
		//UE_LOG(LogTemp, Display, TEXT("Horizontal aspect is: %f, CameraFOV: %f, AspectRatio: %f, LocationZ: %f"), GridAspect, Camera->FieldOfView, AspectRatio, LocationZ);
	} else {
		
		check(AspectRatio);
		LocationZ = (WorldSize.Y * 0.5) / FOVTan(VerticalFOV(Camera->FieldOfView, AspectRatio));
		//UE_LOG(LogTemp, Display, TEXT("Vertical aspect is: %f, CameraFOV: %f, AspectRatio: %f, LocationZ: %f"), GridAspect, Camera->FieldOfView, AspectRatio, LocationZ);
	}
	
	const FVector NewPawnLocation = GridOrigin.GetLocation() +  FVector(WorldSize.Y * 0.5, WorldSize.X * 0.5, LocationZ);
	SetActorLocation(NewPawnLocation);
}





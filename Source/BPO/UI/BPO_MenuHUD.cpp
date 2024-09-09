// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_MenuHUD.h"
#include "UI/BPO_StartGameWidget.h"

void ABPO_MenuHUD::BeginPlay()
{
	Super::BeginPlay();

	StartGameWidget = CreateWidget<UBPO_StartGameWidget>(GetWorld(), StartGameWidgetClass);
	check(StartGameWidget);
	StartGameWidget->AddToViewport();
}

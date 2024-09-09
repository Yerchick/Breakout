// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_MenuHUD.h"
#include "UI/BPO_StartGameWidget.h"
#include "Framework/RequestsSubsystem.h"

void ABPO_MenuHUD::BeginPlay()
{
	Super::BeginPlay();

	StartGameWidget = CreateWidget<UBPO_StartGameWidget>(GetWorld(), StartGameWidgetClass);
	check(StartGameWidget);
	StartGameWidget->AddToViewport();


	auto RequestsSubsys = GetWorld()->GetGameInstance()->GetSubsystem<URequestsSubsystem>();
	if (RequestsSubsys) {
		RequestsSubsys->AnimalFactRequestComplete.AddDynamic(this, &ABPO_MenuHUD::OnFactReceived);
		RequestsSubsys->RequestAnimalFact();
	}
}

void ABPO_MenuHUD::OnFactReceived(FString fact)
{
	if (StartGameWidget) {
		StartGameWidget->ShowAnimalFactInfo(fact);
	}
	UE_LOG(LogTemp, Display, TEXT("OnFactReceived: %s"), *fact);
}


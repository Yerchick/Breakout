// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_StartGameWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"



void UBPO_StartGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(StartGameButton);
	StartGameButton->OnClicked.AddDynamic(this, &ThisClass::ShowAnimaFact);

	check(OkButton);
	OkButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);

	check(ExitButton);
	ExitButton->OnClicked.AddDynamic(this, &ThisClass::OnExitGame);

	WidgetSwitcher.Get()->SetActiveWidgetIndex(0);
}

void  UBPO_StartGameWidget::ShowAnimaFact()
{
	WidgetSwitcher.Get()->SetActiveWidgetIndex(1);
}

void UBPO_StartGameWidget::OnExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void UBPO_StartGameWidget::OnStartGame()
{
	WidgetSwitcher.Get()->SetActiveWidgetIndex(0);
	UGameplayStatics::OpenLevel(GetWorld(), "GameLevel");
}

void UBPO_StartGameWidget::ShowAnimalFactInfo(FString text)
{
	if (AnimalFactsText) {
		AnimalFactsText->SetText(FText::FromString(text));
	}
}
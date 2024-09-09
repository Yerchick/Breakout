// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UBPO_GameOverWidget::UpdateScore(uint32 points)
{
	if (PointsText) {
		PointsText->SetText(FText::FromString(FString::FromInt(points)));
	}
}

void UBPO_GameOverWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(BackToMenuButton);
	BackToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnBackToMenu);
}

void UBPO_GameOverWidget::OnBackToMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenu");
}

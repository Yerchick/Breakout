// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_GameplayWidget.h"
#include "Components/TextBlock.h"

void UBPO_GameplayWidget::UpdateScore(uint32 points)
{
	if (PointsText) {
		PointsText->SetText(FText::FromString(FString::FromInt(points)));
	}
}

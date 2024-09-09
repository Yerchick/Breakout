// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "UI/BPO_StartGameWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UBPO_StartGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(StartGameButton);
	StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnStartGame);
}

void UBPO_StartGameWidget::OnStartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "GameLevel");

}




//auto Subroute = TEXT("https://cat-fact.herokuapp.com");
//TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
//Request->SetURL(Subroute);
//Request->SetTimeout(10);
//Request->SetVerb("GET");
//Request->OnProcessRequestComplete().BindUObject(this, &URequestsSubsystem::GetBuildVersionResponse);
//Request->ProcessRequest();
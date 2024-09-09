// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Framework/RequestsSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(RequestsSubsystemLog, All, All);

void URequestsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Http = &FHttpModule::Get();
}

void URequestsSubsystem::GetAnimalFactRequest()
{
	auto Subroute = TEXT("https://cat-fact.herokuapp.com/facts");
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(Subroute);
	Request->SetTimeout(10);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &URequestsSubsystem::GetAnimalFactResponse);
	Request->ProcessRequest();
}

void URequestsSubsystem::GetAnimalFactResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(RequestsSubsystemLog, Display, TEXT("GetAnimalFactResponse: %s"), *Response.Get()->GetContentAsString());
	} else {
		UE_LOG(RequestsSubsystemLog, Error, TEXT("GetAnimalFactResponse request failed! %s"), *Response.Get()->GetContentAsString());
	}
}



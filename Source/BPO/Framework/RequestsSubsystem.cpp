// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.


#include "Framework/RequestsSubsystem.h"
#include "Json.h"
#include "Serialization/JsonReader.h"

DEFINE_LOG_CATEGORY_STATIC(RequestsSubsystemLog, All, All);

void URequestsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Http = &FHttpModule::Get();
}

void URequestsSubsystem::RequestAnimalFact()
{
	auto Subroute = TEXT("https://cat-fact.herokuapp.com/facts");
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->SetURL(Subroute);
	Request->SetTimeout(10);
	Request->SetVerb("GET");
	Request->OnProcessRequestComplete().BindUObject(this, &URequestsSubsystem::OnAnimalFactResponse);
	Request->ProcessRequest();
	UE_LOG(RequestsSubsystemLog, Display, TEXT("Request sent"));
	AnimalFactRequestComplete.Broadcast("If wait a bit - you'll see interesting fact about animals");
}

void URequestsSubsystem::OnAnimalFactResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful) {
		UE_LOG(RequestsSubsystemLog, Display, TEXT("GetAnimalFactResponse: %s"), *Response.Get()->GetContentAsString());
		auto JSONresponse = TryGetJson(*Response.Get()->GetContentAsString());
		FString resultText = "";
		const FString FieldKey = "text";
		const FStringView sv = FStringView(*FieldKey);
		
		if (!JSONresponse.IsEmpty()) {
			if (JSONresponse[0].Get()->AsObject().Get()->TryGetStringField(FieldKey,resultText)) {
				
				AnimalFactRequestComplete.Broadcast(resultText);
			} 
		}
	} else {
		if (Response.Get()) {
			AnimalFactRequestComplete.Broadcast(LexToString(Response.Get()->GetFailureReason()));
		} else {
			AnimalFactRequestComplete.Broadcast("sorry... \"cat-fact\" API is not responding");
		}
		
		UE_LOG(RequestsSubsystemLog, Error, TEXT("GetAnimalFactResponseFailed"));
	}
}

TArray<TSharedPtr<FJsonValue>>  URequestsSubsystem::TryGetJson(FString jsonString)
{
	TArray<TSharedPtr<FJsonValue>> MyJson;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(jsonString);
	if (FJsonSerializer::Deserialize(Reader, MyJson)) {
		//UE_LOG(RequestsSubsystemLog, Error, TEXT("Deserialization failed! %s"), *jsonString);
		return MyJson;
	}
	return MyJson;
}



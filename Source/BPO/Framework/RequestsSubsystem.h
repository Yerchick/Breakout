// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Http.h"
#include "RequestsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BPO_API URequestsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFactAboutAnimalRequestCompleteDelegate, bool, Succeeded);
	UPROPERTY(BlueprintAssignable, Category = "Fact");
	FFactAboutAnimalRequestCompleteDelegate AnimalFactRequestComplete;
	UFUNCTION(BlueprintCallable)
	void GetAnimalFactRequest();
	void GetAnimalFactResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:

	FHttpModule* Http;
};

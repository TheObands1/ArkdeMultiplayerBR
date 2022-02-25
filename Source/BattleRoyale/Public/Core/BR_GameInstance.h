// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Templates/SharedPointer.h"
#include "BR_GameInstance.generated.h"

/**
 * 
 */
class IOnlineSession;

UCLASS()
class BATTLEROYALE_API UBR_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	FName SessionDefaultName = TEXT("SessionDefaultName");

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;


public:

	virtual void Init() override;

	void OnSessionCreated(FName SessionName, bool bWasSuccessful);

	void OnSessionDestroy(FName SessionName, bool Success);

	void OnSessionsFound(bool Success);

	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	UFUNCTION(BlueprintCallable)
	void HostGame();

	UFUNCTION(BlueprintCallable)
	void JoinGame();
	
};

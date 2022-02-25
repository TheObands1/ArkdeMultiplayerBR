// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/BR_GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

void UBR_GameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::Init Subsystem Found %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance:: Init Session Interface Found"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UBR_GameInstance::OnSessionCreated);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UBR_GameInstance::OnSessionDestroy);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UBR_GameInstance::OnSessionsFound);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UBR_GameInstance::OnSessionJoined);
		}
	}
}

void UBR_GameInstance::OnSessionCreated(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionCreated Create session end"));
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionCreated Create session Failed"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionCreated Create session Success"));
	UWorld* GameWorld = GetWorld();
	GameWorld->ServerTravel("/Game/Maps/Lobby?listen");
}

void UBR_GameInstance::OnSessionDestroy(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionDestroyed Destroy Session End"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionDestroyed Destroy Session Failed"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionDestroyed Destroy Session Success"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);


}

void UBR_GameInstance::OnSessionsFound(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionsFound Find Sessions end"));

	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionsFound Find Sessions Failed"));
		return;
	}

	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (FOnlineSessionSearchResult CurrentResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionsFound Session Found %s"), *CurrentResult.GetSessionIdStr());
		}

		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionsFound Join Session Start"));
		SessionInterface->JoinSession(0, SessionDefaultName, SessionSearch->SearchResults[0]);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionsFound No Sessions Found"));
	}
}

void UBR_GameInstance::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionJoined Join Sessions End"));

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionJoined Join Sessions Failed"));
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::OnSessionJoined No Player Controller Found"));
		return;
	}

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UBR_GameInstance::HostGame()
{
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::Host Create Session Start"));
	auto CreatedSession = SessionInterface->GetNamedSession(SessionDefaultName);

	if (CreatedSession)
	{
		SessionInterface->DestroySession(SessionDefaultName);
	}
	else
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.NumPublicConnections = 2;
		SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);
	}


	
}

void UBR_GameInstance::JoinGame()
{
	UE_LOG(LogTemp, Warning, TEXT("UBR_GameInstance::JoinGame Find Session Start"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->bIsLanQuery = true;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}



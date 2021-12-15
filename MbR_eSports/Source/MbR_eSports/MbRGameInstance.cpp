// Fill out your copyright notice in the Description page of Project Settings.


#include "MbRGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMbRGameInstance::UMbRGameInstance()
{
}

void UMbRGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnJoinSessionComplete);
		}
	}
}

void UMbRGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/DefaultTestMap?listen");
	}
}

void UMbRGameInstance::OnFindSessionComplete(bool Succeeded)
{
	if (Succeeded)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;

		if (SearchResults.Num())
		{
			//Hard Coding the first session result here for testing
			SessionInterface->JoinSession(0, FName("MbR Session"), SearchResults[0]);
		}
	}
}

void UMbRGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UMbRGameInstance::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("MbR Session"));
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("MbR Session"), SessionSettings);
}

void UMbRGameInstance::JoinServer()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set("SEARCH_PRESENCE", true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}
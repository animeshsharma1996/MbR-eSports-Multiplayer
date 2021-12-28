#include "MbRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

UMbRGameInstance::UMbRGameInstance() {}

void UMbRGameInstance::Init()
{
	FScriptDelegate serverCreationDel;
	serverCreationDel.BindUFunction(this, "RemoveMainMenuFromViewport");

	serverCreation.Add(serverCreationDel);

	if (IOnlineSubsystem* subSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = subSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnJoinSessionComplete);
		}
	}
}

void UMbRGameInstance::CreateServer(FString serverName, FString hostName)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Server"));

	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 5;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), serverName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), hostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	/*SessionSettings.NumPrivateConnections = 5;
	SessionSettings.bAllowInvites = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;*/

	SessionInterface->CreateSession(0, FName("Game Session"), SessionSettings);
}

void UMbRGameInstance::FindServers()
{
	UE_LOG(LogTemp, Warning, TEXT("Find Server"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMbRGameInstance::OnCreateSessionComplete(FName ServerName, bool Succeessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), Succeessful);
	if (Succeessful)
	{
		serverCreation.Broadcast(Succeessful);
		GetWorld()->ServerTravel("/Game/_Maps/DefaultTestMap?listen");
		//UGameplayStatics::OpenLevel(GetWorld(), "DefaultTestMap", true, "listen");
	}
}

void UMbRGameInstance::OnFindSessionsComplete(bool Succeessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), Succeessful);
	if (Succeessful)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		OnAssignSearchResults(SearchResults);

		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SearchResults.Num());

	/*	if (SearchResults.Num())
		{
			SessionInterface->JoinSession(0, "Game Session", SearchResults[0]);
		}*/
	}
}

void UMbRGameInstance::OnAssignSearchResults(TArray<FOnlineSessionSearchResult> searchResults)
{
	for (FOnlineSessionSearchResult result : searchResults)
	{
		if (!result.IsValid()) { continue; }

		FServerInfo serverInfo;
		FString serverName = "Empty Server Name";
		FString hostName = "Empty Host Name";

		result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), serverName);
		result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), hostName);

		serverInfo.serverName = serverName;
		serverInfo.maxPlayers = result.Session.SessionSettings.NumPublicConnections;
		serverInfo.currentPlayers = serverInfo.maxPlayers - result.Session.NumOpenPublicConnections;
		serversListDel.Broadcast(serverInfo);
	}
}

void UMbRGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionsComplete, SessionName: %s"),*SessionName.ToString());
	if (APlayerController* pController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString joinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, joinAddress);
		if (joinAddress != "")
		{
			pController->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}
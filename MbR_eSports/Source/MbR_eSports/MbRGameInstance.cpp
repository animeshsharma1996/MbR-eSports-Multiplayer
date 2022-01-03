#include "MbRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

UMbRGameInstance::UMbRGameInstance() 
{
	defaultSessionName = FName("Game Session");
}

void UMbRGameInstance::Init()
{
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

void UMbRGameInstance::CreateServer(FString sessionName, FString hostName)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Server"));

	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 10;
	SessionSettings.NumPrivateConnections = 10;

	SessionSettings.bAllowInvites = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), sessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("SERVER_HOSTNAME_KEY"), hostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (sessionName.IsEmpty())
	{
		defaultSessionName = FName(sessionName);
	}

	SessionInterface->CreateSession(0, defaultSessionName, SessionSettings);
}

void UMbRGameInstance::FindServers()
{
	searchingForServers.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("Find Server"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMbRGameInstance::JoinServer(int32 arrayIndex, FName joinSessionName)
{
	FOnlineSessionSearchResult result = SessionSearch->SearchResults[arrayIndex];
	if (result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Joining Server at Index: %d"), arrayIndex);
		if (joinSessionName.ToString().IsEmpty())
		{
			SessionInterface->JoinSession(0, defaultSessionName, result);
		}
		else
		{
			SessionInterface->JoinSession(0, joinSessionName, result);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Join Server at Index: %d"), arrayIndex);
	}
}

void UMbRGameInstance::OnCreateSessionComplete(FName serverName, bool succeessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), succeessful);
	if (succeessful)
	{
		serverCreation.Broadcast(succeessful);
		GetWorld()->ServerTravel("/Game/_Maps/DefaultTestMap?listen");
		UGameplayStatics::OpenLevel(GetWorld(), "DefaultTestMap", true, "listen");
	}
}

void UMbRGameInstance::OnFindSessionsComplete(bool succeessful)
{
	searchingForServers.Broadcast(false);

	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), succeessful);
	if (succeessful)
	{
		OnAssignSearchResults();
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SessionSearch->SearchResults.Num());
	}
}

void UMbRGameInstance::OnAssignSearchResults()
{
	int32 serverArrayIndex = 0;
	for (FOnlineSessionSearchResult result : SessionSearch->SearchResults)
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
		serverInfo.isLan = result.Session.SessionSettings.bIsLANMatch;
		serverInfo.ping = result.PingInMs;
		serverInfo.serverArrayIndex = serverArrayIndex;
		serversListDel.Broadcast(serverInfo);

		++serverArrayIndex;
	}
}

void UMbRGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionsComplete, SessionName: %s"),*sessionName.ToString());
	if (APlayerController* pController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString joinAddress = "";
		SessionInterface->GetResolvedConnectString(sessionName, joinAddress);
		if (!joinAddress.IsEmpty())
		{
			pController->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}
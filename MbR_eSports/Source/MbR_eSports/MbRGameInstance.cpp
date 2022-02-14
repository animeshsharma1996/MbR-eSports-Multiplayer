#include "MbRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"

//Constructor to define default session name and the online subsystem
UMbRGameInstance::UMbRGameInstance() 
{
	defaultSessionName = FName("MbR_Game Session");
	onlineSubsystem = IOnlineSubsystem::Get();
}

//Initialise the GameInstance and SessionInterface by binding the inherited and inbuilt functions to the session interface
void UMbRGameInstance::Init()
{
	if (onlineSubsystem)
	{
		SessionInterface = onlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnFindSessionsComplete);
			//SessionInterface->OnFindFriendSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnFindFriendSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnJoinSessionComplete);
		}
	}
}

//Create a server by passing custom server info through the custom server menu
void UMbRGameInstance::CreateServer(FPassedServerInfo passedServerInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("Create Server"));

	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = passedServerInfo.maxPlayers;
	SessionSettings.NumPrivateConnections = passedServerInfo.maxPlayers;

	SessionSettings.Set(FName("SERVER_NAME_KEY"), passedServerInfo.serverName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (!passedServerInfo.serverName.IsEmpty())
	{
		defaultSessionName = FName(passedServerInfo.serverName);
	}

	SessionInterface->CreateSession(0, defaultSessionName, SessionSettings);
}

//Find servers function, called when server list is opened or the refresh button in the menu is clicked
void UMbRGameInstance::FindServers()
{
	searchingForServers.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("Find Server"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

//Find friends servers function, called when friends server list is opened or the refresh button in the menu is clicked
void UMbRGameInstance::FindServersOfFriends()
{
    UE_LOG(LogTemp, Warning, TEXT("Find Friends' Server"));
    
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
    SessionSearch->MaxSearchResults = 1000;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    //SessionInterface->FindFriendSession(0, SessionSearch.ToSharedRef());
}

//Join server according to the server slot, called when join button is clicked
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

/*
Delegate function, called when a session creation is completed. Makes the server and the relevant client travel to the 
DefaultTestMap. The map name is hard coded here, obviously, this should be changed accordingly and passed as a variable.
The variable can be assigned in the child blueprint class
*/
void UMbRGameInstance::OnCreateSessionComplete(FName serverName, bool successful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), successful);
	if (successful)
	{
		serverCreation.Broadcast(successful);
		UGameplayStatics::OpenLevel(GetWorld(), "DefaultTestMap", true, "listen");
	}
}

//Delegate function, called when sessions search is completed
void UMbRGameInstance::OnFindSessionsComplete(bool successful)
{
	searchingForServers.Broadcast(false);

	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete, Succeeded: %d"), successful);
	if (successful)
	{
		OnAssignSearchResults();
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SessionSearch->SearchResults.Num());
	}
}

//Delegate function, called when friends' sessions search is completed
void UMbRGameInstance::OnFindFriendSessionComplete(bool successful)
{
    searchingForServers.Broadcast(false);

	UE_LOG(LogTemp, Warning, TEXT("OnFindFriendSessionsComplete, Succeeded: %d"), successful);
	if (successful)
	{
		OnAssignSearchResults();
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), SessionSearch->SearchResults.Num());
	}
}

//Function called to assign search results upon completion of sessions search
void UMbRGameInstance::OnAssignSearchResults()
{
	int32 serverArrayIndex = 0;
	for (FOnlineSessionSearchResult result : SessionSearch->SearchResults)
	{
		if (!result.IsValid()) { continue; }

		FServerInfo serverInfo;
		FString serverName = "Empty Server Name";

		result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), serverName);

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

//Delegate function, called when session join is completed. This effectively makes the client travel to the main map
void UMbRGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionsComplete, SessionName: %s"),*sessionName.ToString());
	if (APlayerController* playerController = GetFirstLocalPlayerController())
	{
		if (!SessionInterface.IsValid()) { return; }

		FString joinAddress = "";
		if (!SessionInterface->GetResolvedConnectString(sessionName, joinAddress))
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
			return;
		}

		if (!joinAddress.IsEmpty())
		{
			playerController->ClientTravel(joinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}
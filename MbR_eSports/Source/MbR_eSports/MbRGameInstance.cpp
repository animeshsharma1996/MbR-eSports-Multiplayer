#include "MbRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSharingInterface.h"
#include "Templates/SharedPointer.h"

//Constructor to define default session name and the online subsystem
UMbRGameInstance::UMbRGameInstance() 
{
	defaultSessionName = FName("Custom Name");
	onlineSubsystem = IOnlineSubsystem::Get();

	if (onlineSubsystem)
	{
		friendInterface = onlineSubsystem->GetFriendsInterface();
		localUserId = (onlineSubsystem->GetIdentityInterface()->GetUniquePlayerId(0)).Get();
		FOnReadFriendsListComplete friendDelegate;
		friendDelegate.BindUObject(this, &UMbRGameInstance::OnReadFriendsComplete);
		if (friendInterface.IsValid())
		{
			friendInterface->ReadFriendsList(0, friendListName, friendDelegate);
		}
	}
}

//Initialise the GameInstance and SessionInterface by binding the inherited and inbuilt functions to the session interface
void UMbRGameInstance::Init()
{
	if (onlineSubsystem)
	{
		sessionInterface = onlineSubsystem->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnCreateSessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnJoinSessionComplete);
			sessionInterface->OnEndSessionCompleteDelegates.AddUObject(this, &UMbRGameInstance::OnEndSessionComplete);
		}
	}
}

void UMbRGameInstance::AssignMapNames(FName lobbyMap, FName mainMenuMap)
{
	lobbyMapName = lobbyMap;
	mainMenuMapName = mainMenuMap;
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

	sessionInterface->CreateSession(0, defaultSessionName, SessionSettings);
}

//Find servers function, called when server list is opened or the refresh button in the menu is clicked
void UMbRGameInstance::FindServers()
{
	searchingForServers.Broadcast(true);

	UE_LOG(LogTemp, Warning, TEXT("Find Server"));

	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	sessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
	sessionSearch->MaxSearchResults = 9999;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

//Find friends servers function, called when friends server list is opened or the refresh button in the menu is clicked
void UMbRGameInstance::FindServersOfFriends()
{
    UE_LOG(LogTemp, Warning, TEXT("Find Friends' Server"));
    
    sessionSearch = MakeShareable(new FOnlineSessionSearch());
    sessionSearch->bIsLanQuery = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") ? true : false;
    sessionSearch->MaxSearchResults = 1000;
    sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    sessionInterface->FindFriendSession(*localUserId, friendList);
}

//Join server according to the server slot, called when join button is clicked
void UMbRGameInstance::JoinServer(int32 arrayIndex, FName joinSessionName)
{
	FOnlineSessionSearchResult result = sessionSearch->SearchResults[arrayIndex];
	if (result.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Joining Server at Index: %d"), arrayIndex);
		if (joinSessionName.ToString().IsEmpty())
		{
			sessionInterface->JoinSession(0, defaultSessionName, result);
		}
		else
		{
			sessionInterface->JoinSession(0, joinSessionName, result);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Join Server at Index: %d"), arrayIndex);
	}
}

//Should end the server when the match is finished or when the host leaves the lobby/match
void UMbRGameInstance::EndServer()
{
    sessionInterface->EndSession(defaultSessionName);
}

/*
Delegate function, called when a session creation is completed. Makes the server and the relevant client travel to the 
lobbyMap
*/
void UMbRGameInstance::OnCreateSessionComplete(FName serverName, bool successful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded: %d"), successful);
	if (successful && !lobbyMapName.ToString().IsEmpty())
	{
		serverCreation.Broadcast(successful);
		UGameplayStatics::OpenLevel(GetWorld(), lobbyMapName, true, "listen");
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
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), sessionSearch->SearchResults.Num());
	}
}

//Delegate function, called when friends' list read is completed
void UMbRGameInstance::OnReadFriendsComplete(int32 localPlayer, bool successful, const FString& listName, const FString& errorStr)
{
	if (successful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnReadFriendsComplete, Succeeded: %d"), successful);
		friendInterface->GetFriendsList(localPlayer, listName, onlineFriendList);
		for (TSharedRef<FOnlineFriend> onlineFriend : onlineFriendList)
		{
			friendList.Add(onlineFriend->GetUserId());
		}
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
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Server Count: %d"), sessionSearch->SearchResults.Num());
	}
}

//Function called to assign search results upon completion of sessions search
void UMbRGameInstance::OnAssignSearchResults()
{
	int32 serverArrayIndex = 0;
	for (FOnlineSessionSearchResult result : sessionSearch->SearchResults)
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
		if (!sessionInterface.IsValid()) { return; }

		FString joinAddress = "";
		if (!sessionInterface->GetResolvedConnectString(sessionName, joinAddress))
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

//Delegate function, called when a session/server is ended after the match is over
void UMbRGameInstance::OnEndSessionComplete(FName sessionName, bool successful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnEndSessionComplete, Succeeded: %d"), successful);

    for(FConstPlayerControllerIterator pControllerIt = GetWorld()->GetPlayerControllerIterator(); pControllerIt; ++pControllerIt)
    {
        APlayerController* playerControllerClient = Cast<APlayerController>(pControllerIt->Get());
        if(playerControllerClient && !playerControllerClient->IsLocalPlayerController())
        {
			UGameplayStatics::OpenLevel(GetWorld(), mainMenuMapName, true, "listen");
        }
    }
	UGameplayStatics::OpenLevel(GetWorld(), mainMenuMapName, true, "listen");
	sessionInterface->DestroySession(defaultSessionName);
}
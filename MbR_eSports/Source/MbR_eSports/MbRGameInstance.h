#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
#include "Components/TextRenderComponent.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MbRGameInstance.generated.h"

/*
The Game Instance class derived from the UGameInstance to create, find and join servers with the help of delegates 
and prebuild virtual functions such as OnCreateSessionComplete, OnFindSessionsComplete, etc.
The variables and functions names are self explanatory.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateServer, FServerInfo, serversListDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCreation, bool, successful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateServerSearching, bool, searchingForServers);

UCLASS()
class MBR_ESPORTS_API UMbRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMbRGameInstance();

	UFUNCTION(BlueprintCallable)
		void AssignMapNames(FName lobbyMap, FName mainMenuMap);
	UFUNCTION(BlueprintCallable)
		void CreateServer(FPassedServerInfo passedServerInfo);
	UFUNCTION(BlueprintCallable)
		void FindServers();		
	UFUNCTION(BlueprintCallable)
		void FindServersOfFriends();		
	UFUNCTION(BlueprintCallable)
		void JoinServer(int32 arrayIndex, FName joinSessionName);	
	UFUNCTION(BlueprintCallable)
		void EndServer();

	UPROPERTY(BlueprintAssignable)
		FDelegateServer serversListDel;	
	UPROPERTY(BlueprintAssignable)
		FDelegateCreation serverCreation;	
	UPROPERTY(BlueprintAssignable)
		FDelegateServerSearching searchingForServers;

protected:
	IOnlineSessionPtr sessionInterface;
	IOnlineFriendsPtr friendInterface;
	TSharedPtr<FOnlineSessionSearch> sessionSearch;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName sessionName, bool successful);
	virtual void OnFindSessionsComplete(bool successful);
	virtual void OnFindFriendSessionComplete(bool successful);
	virtual void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	virtual void OnEndSessionComplete(FName sessionName, bool successful);

	UFUNCTION()
		void OnAssignSearchResults();
	UPROPERTY()
		FName defaultSessionName;

private:
	IOnlineSubsystem* onlineSubsystem;
	TArray<TSharedRef<FOnlineFriend>> onlineFriendList;
	TArray<TSharedRef<const FUniqueNetId>> friendList;
	const FUniqueNetId* localUserId;
	UPROPERTY()
		FName lobbyMapName;
	UPROPERTY()
		FName mainMenuMapName;
	UPROPERTY()
		FString friendListName;
};

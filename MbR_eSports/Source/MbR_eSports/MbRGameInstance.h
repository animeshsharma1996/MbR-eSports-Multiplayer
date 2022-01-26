#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
#include "Components/TextRenderComponent.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MbRGameInstance.generated.h"

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
		void CreateServer(FPassedServerInfo passedServerInfo);
	UFUNCTION(BlueprintCallable)
		void FindServers();		
	UFUNCTION(BlueprintCallable)
		void JoinServer(int32 arrayIndex, FName joinSessionName);
	UPROPERTY(BlueprintAssignable)
		FDelegateServer serversListDel;	
	UPROPERTY(BlueprintAssignable)
		FDelegateCreation serverCreation;	
	UPROPERTY(BlueprintAssignable)
		FDelegateServerSearching searchingForServers;

protected:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName sessionName, bool successful);
	virtual void OnFindSessionsComplete(bool successful);
	virtual void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	UFUNCTION()
		void OnAssignSearchResults();
	UPROPERTY()
		FName defaultSessionName;

private:
	IOnlineSubsystem* onlineSubsystem;
};

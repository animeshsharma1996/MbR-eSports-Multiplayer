#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
#include "MainMenu/ServerSlotWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MbRGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateServer, FServerInfo, serversListDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateCreation, bool, successful);

UCLASS()
class MBR_ESPORTS_API UMbRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMbRGameInstance();

	UFUNCTION(BlueprintCallable)
		void CreateServer(FString serverName, FString hostName);
	UFUNCTION(BlueprintCallable)
		void FindServers();	
	UPROPERTY(BlueprintAssignable)
		FDelegateServer serversListDel;	
	UPROPERTY(BlueprintAssignable)
		FDelegateCreation serverCreation;

protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeessful);
	virtual void OnFindSessionsComplete(bool Succeessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnAssignSearchResults(TArray<FOnlineSessionSearchResult> searchResults);
};

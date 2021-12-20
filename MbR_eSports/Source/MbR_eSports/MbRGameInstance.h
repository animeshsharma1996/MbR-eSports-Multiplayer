#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "MbRGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
public :
	UPROPERTY(BlueprintReadOnly)
		FString serverName;
	UPROPERTY(BlueprintReadOnly)
		int32 currentPlayers;
	UPROPERTY(BlueprintReadOnly)
		int32 maxPlayers;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);

UCLASS()
class MBR_ESPORTS_API UMbRGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMbRGameInstance();


	UFUNCTION(BlueprintCallable)
		void CreateServer();
	UFUNCTION(BlueprintCallable)
		void JoinServer();
	UPROPERTY(/*BlueprintAssignable*/)
		FServerDel serverListDel;

protected:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName ServerName, bool Succeessful);
	virtual void OnFindSessionsComplete(bool Succeessful);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
};

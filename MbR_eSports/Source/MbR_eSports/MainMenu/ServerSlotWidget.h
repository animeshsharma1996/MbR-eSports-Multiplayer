#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
#include "Blueprint/UserWidget.h"
#include "MbR_eSports/MbRGameInstance.h"
#include "ServerSlotWidget.generated.h"

/*
Server Slot Widget class created to contain the line slot of server data which will be displayed when servers are found.
Each slot line will have server name, players numbers, lan info, ping and join button
*/

UCLASS()
class MBR_ESPORTS_API UServerSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadOnly)
		struct FServerInfo serverData;
	UFUNCTION()
		void OnServerInfoUpdate(FServerInfo serverInfo, UMbRGameInstance* gameInstance);

protected :
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* serverNameText;	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* playersNumText;		
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* lanText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* pingText;	
	UPROPERTY(meta = (BindWidget))
		class UButton* joinButton;
	UPROPERTY()
		int32 arrayIndex;
	UFUNCTION()
		void OnJoinButtonClicked();

private :
	UPROPERTY()
		class UMbRGameInstance* mbRGameInstance;
};

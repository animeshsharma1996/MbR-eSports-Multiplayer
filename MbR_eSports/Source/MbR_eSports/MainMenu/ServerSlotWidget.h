#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
#include "Blueprint/UserWidget.h"
#include "MbR_eSports/MbRGameInstance.h"
#include "ServerSlotWidget.generated.h"

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
		class UButton* joinButton;
	UPROPERTY()
		int32 arrayIndex;
	UFUNCTION()
		void OnJoinButtonClicked();

private :
	UPROPERTY()
		class UMbRGameInstance* mbRGameInstance;
};

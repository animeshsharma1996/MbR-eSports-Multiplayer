#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/MbRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "ServerSlotWidget.generated.h"

UCLASS()
class MBR_ESPORTS_API UServerSlotWidget : public UUserWidget
{
	GENERATED_BODY()
		virtual bool Initialize() override;

protected :
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* serverNameText;	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* playersNumText;
	UPROPERTY()
		FServerInfo serverData;
private :

};

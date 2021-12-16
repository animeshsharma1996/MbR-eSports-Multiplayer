#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MbR_eSports/MbRGameInstance.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class MBR_ESPORTS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
		virtual bool Initialize() override;

public :
	UPROPERTY(meta = (BindWidget))
		class UButton* hostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* connectButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* exitButton;
	UPROPERTY()
		class UMbRGameInstance* mbRGameInstance;

	UFUNCTION()
		void HostButtonClicked();
	UFUNCTION()
		void ConnectButtonClicked();
	UFUNCTION()
		void ExitButtonClicked();
};

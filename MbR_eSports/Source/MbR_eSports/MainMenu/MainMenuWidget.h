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

protected :
	UPROPERTY(meta = (BindWidget))
		class UButton* hostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* connectButton;	
	UPROPERTY(meta = (BindWidget))
		class UButton* refreshServersButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* backButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* exitButton;	
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* widgetSwitcherServerList;
	UPROPERTY()
		class UMbRGameInstance* mbRGameInstance;

private :
	UFUNCTION()
		void OnHostButtonClicked();
	UFUNCTION()
		void OnServersListButtonClicked();	
	UFUNCTION()
		void OnRefreshServersButtonClicked();	
	UFUNCTION()
		void OnBackButtonClicked();
	UFUNCTION()
		void OnExitButtonClicked();
};

#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
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
		class UButton* customServerButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* serversListButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* refreshServersButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* backButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* exitButton;		
	UPROPERTY(meta = (BindWidget))
		class UButton* customHostButton;	
	UPROPERTY(meta = (BindWidget))
		class UButton* customServerBackButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* widgetSwitcherServerList;		
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* serverListScrollBox;	
	UPROPERTY(meta = (BindWidget))
		class UEditableText* serverNameTextBox;

	UPROPERTY(meta = (BindWidget))
		class USlider* maxPlayersNumSlider;
	UPROPERTY(meta = (BindWidget))
		class UCheckBox* lanCheckBox;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* maxPlayersNumText;

	UPROPERTY(BlueprintReadOnly)
		class UMbRGameInstance* mbRGameInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
		TSubclassOf<UUserWidget> serverSlotWidget;

private :
	UFUNCTION()
		void OnCustomServerButtonClicked();
	UFUNCTION()
		void OnServersListButtonClicked();	
	UFUNCTION()
		void OnRefreshServersButtonClicked();	
	UFUNCTION()
		void OnHostCustomServerButtonClicked();
	UFUNCTION()
		void OnMaxPlayersSliderChanged(float value);
	UFUNCTION()
		void CreateServerSlotWidget(FServerInfo serverInfo);
	UFUNCTION()
		void SearchingForServers(bool isSearching);
	UFUNCTION()
		void OnBackButtonClicked();
	UFUNCTION()
		void OnExitButtonClicked();
	UPROPERTY()
		bool initialSearchForServers;
};

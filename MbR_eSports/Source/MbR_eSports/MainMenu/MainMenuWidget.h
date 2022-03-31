#pragma once

#include "CoreMinimal.h"
#include "MbR_eSports/ServerInfoStruct.h"
#include "Blueprint/UserWidget.h"
#include "MbR_eSports/MbRGameInstance.h"
#include "MainMenuWidget.generated.h"

/*
The MainMenuWidget class derived from UserWidget to bind and create functionalities of UI elements such as buttons,
texts, scrollbox, slider, checkbox created in the MainMenu along with the widget switcher.
The variables and functions names are self explanatory.
*/

UCLASS()
class MBR_ESPORTS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
		virtual bool Initialize() override;

public :
    UFUNCTION()
        void InGameMenu(); 
	UFUNCTION()
        void PublicTick();

protected :
	UPROPERTY(meta = (BindWidget))
		class UButton* customServerButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* serversListButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* friendsServersListButton;
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
	    class UButton* inGameMenuBackButton;
	UPROPERTY(meta = (BindWidget))
	    class UButton* cancelButton;
	UPROPERTY(meta = (BindWidget))
	    class UButton* inGameMenuExitButton;

    UPROPERTY(meta = (BindWidget))
        class UImage* backgroundImage;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* widgetSwitcherServerList;		
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* serverListScrollBox;	
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* serverNameEditableTextBox;

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
		void OnBackToMainMenuButtonClicked();
	UFUNCTION()	
		void OnCancelButtonClicked();
	UFUNCTION()
		void OnExitButtonClicked();
	UFUNCTION()
		void OnServerEnded(bool ended);
	UPROPERTY()
		bool initialSearchForServers;	
	UPROPERTY()
		bool isServersListPressed;	
	UPROPERTY()
		bool isFriendsListPressed;
	UPROPERTY()
		bool isServerEnded;
};

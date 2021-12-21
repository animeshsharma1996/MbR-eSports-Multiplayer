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

public :
	UFUNCTION()
		void SetServerSlotWidget(TSubclassOf<UUserWidget> widget);

protected :
	UPROPERTY(meta = (BindWidget))
		class UButton* hostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* serversListButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* refreshServersButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* backButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* exitButton;	
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* widgetSwitcherServerList;	
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* serverListScrollBox;
	UPROPERTY(BlueprintReadOnly)
		class UMbRGameInstance* mbRGameInstance;

private :
	UFUNCTION()
		void OnHostButtonClicked();
	UFUNCTION()
		void OnServersListButtonClicked();	
	UFUNCTION()
		void OnRefreshServersButtonClicked();
	void CreateServerSlotWidget();
	UFUNCTION()
		void OnBackButtonClicked();
	UFUNCTION()
		void OnExitButtonClicked();

	UPROPERTY()
		TSubclassOf<UUserWidget> serverSlotWidget;
};

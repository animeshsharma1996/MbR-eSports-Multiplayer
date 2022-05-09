#include "MainMenuWidget.h"
#include "ServerSlotWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Components/Image.h"
#include "Components/Widget.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime/Core/Public/Windows/COMPointer.h>

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	initialSearchForServers = false;
	isServersListPressed = false;
	isFriendsListPressed = false;
	listMenuIndex = 0;
	
	//Bind each button variable defined in the header file (with "meta = (BindWidget)") with relevant function dynamically
	customServerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCustomServerButtonClicked);
	serversListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnServersListButtonClicked);
	friendsServersListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnFriendsListButtonClicked);
	refreshServersButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnRefreshServersButtonClicked);
	customHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostCustomServerButtonClicked);
	backButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	customServerBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
    inGameMenuBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackToMainMenuButtonClicked);
    cancelButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCancelButtonClicked);
    inGameMenuExitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
    
    /*
    Logic to bind delegates to their relevant functions. Servers List and Servers Searching are added to the 
    game instance counter parts
    */
	FScriptDelegate serversListDelegate;
	FScriptDelegate serversSearchingDelegate;
	FScriptDelegate sliderChangeDelegate;
	FScriptDelegate serverEndDelegate;
	serversListDelegate.BindUFunction(this, "CreateServerSlotWidget");
	serversSearchingDelegate.BindUFunction(this, "SearchingForServers");
	sliderChangeDelegate.BindUFunction(this, "OnMaxPlayersSliderChanged");
	serverEndDelegate.BindUFunction(this, "OnServerEnded");
	maxPlayersNumSlider->OnValueChanged.Add(sliderChangeDelegate);
	world = GetWorld();
	playerController = world->GetFirstPlayerController();
	mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(world));
	if (mbRGameInstance != nullptr)
	{
		mbRGameInstance->serversListDel.Add(serversListDelegate);
		mbRGameInstance->searchingForServers.Add(serversSearchingDelegate);
		mbRGameInstance->endServerDel.Add(serverEndDelegate);
		if (mbRGameInstance->onlineSubsystem->GetSubsystemName() == "NULL")
		{
			friendsServersListButton->SetIsEnabled(false);
		}
	}
	return true;
}

//Bring up the in-game menu along with it's functionality 
void UMainMenuWidget::InGameMenu()
{
    backgroundImage->SetVisibility(ESlateVisibility::Hidden);
    widgetSwitcherServerList->SetActiveWidgetIndex(3);
}

//Takes the player to host menu which can be used to host custom server
void UMainMenuWidget::OnCustomServerButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(2);
}

//Takes the player to the server list menu
void UMainMenuWidget::OnServersListButtonClicked()
{
	isServersListPressed = true;
	initialSearchForServers = true;
	listMenuIndex = 1;
	widgetSwitcherServerList->SetActiveWidgetIndex(1);
	RefreshOrSearchServers();
}

//Takes the player to the friends' servers list menu
void UMainMenuWidget::OnFriendsListButtonClicked()
{
	isFriendsListPressed = true;
	initialSearchForServers = true;
	listMenuIndex = 2;
	widgetSwitcherServerList->SetActiveWidgetIndex(1);
	RefreshOrSearchServers();
}

//Clears the list of the servers/friends' servers and researches to find servers/friends' servers
void UMainMenuWidget::OnRefreshServersButtonClicked()
{	
	initialSearchForServers = true;
	switch (listMenuIndex)
	{
	case 1: 
		isServersListPressed = true;
		isFriendsListPressed = false;
		break;
	case 2: 
		isServersListPressed = false;
		isFriendsListPressed = true;
		break;
	}
	RefreshOrSearchServers();
}

//Refresh the servers list or search servers depending upon the button click
void UMainMenuWidget::RefreshOrSearchServers()
{
	if (initialSearchForServers)
	{
		serverListScrollBox->ClearChildren();
		if (mbRGameInstance != nullptr)
		{
			if (isServersListPressed)
			{
				mbRGameInstance->FindServers();
				isServersListPressed = false;
			}

			if (isFriendsListPressed)
			{
				mbRGameInstance->FindServersOfFriends();
				isFriendsListPressed = false;
			}
		}
	}
}

/*
Create a custom server when the host button is clicked. Get the information of the server from the entered data through 
the custom server menu
*/
void UMainMenuWidget::OnHostCustomServerButtonClicked()
{
	FPassedServerInfo passedServerInfo;
	passedServerInfo.serverName = serverNameEditableTextBox->GetText().ToString();
	passedServerInfo.maxPlayers = FGenericPlatformMath::RoundToInt(maxPlayersNumSlider->GetValue());
	passedServerInfo.isLan = lanCheckBox->IsChecked();

	if (mbRGameInstance != nullptr)
	{
		mbRGameInstance->CreateServer(passedServerInfo);
	}
}

//Increase or decrease the max players according to the player slider
void UMainMenuWidget::OnMaxPlayersSliderChanged(float value)
{
	maxPlayersNumText->SetText(FText::AsCultureInvariant(FString::FromInt(FGenericPlatformMath::RoundToInt(value))));
}

//Create server slot widget with the help of the passed parameter containing the custom created server info
void UMainMenuWidget::CreateServerSlotWidget(FServerInfo serverInfo)
{
	if (serverSlotWidget != nullptr)
	{
		UUserWidget* serverSlotUserWidget = CreateWidget<UUserWidget>(GetWorld(), serverSlotWidget);
		if (serverSlotUserWidget != nullptr && mbRGameInstance != nullptr)
		{
			Cast<UServerSlotWidget>(serverSlotUserWidget)->OnServerInfoUpdate(serverInfo, mbRGameInstance);
			serverListScrollBox->AddChild(serverSlotUserWidget);
		}
	}
}

//Search for servers when refresh button is clicked. Also disable the refresh button while searching
void UMainMenuWidget::SearchingForServers(bool isSearching)
{
	if (isSearching)
	{
		refreshServersButton->SetIsEnabled(false);
	}
	else
	{
		refreshServersButton->SetIsEnabled(true);
	}
}

//Back button (from host screen/join screen) takes the player back to the main menu 
void UMainMenuWidget::OnBackButtonClicked()
{
	backgroundImage->SetVisibility(ESlateVisibility::Visible);
	listMenuIndex = 0;
	widgetSwitcherServerList->SetActiveWidgetIndex(0);

}

//Back button (from in-game menu) takes the player back to the main menu
void UMainMenuWidget::OnBackToMainMenuButtonClicked()
{
	if (mbRGameInstance != nullptr)
	{
		mbRGameInstance->EndServer();

		//if (isServerEnded)
		//{
		//	OnBackButtonClicked();
		//}
	}
	isServersListPressed = false;
	isFriendsListPressed = false;
}

//Cancel Button removes the in game menu and unpauses for the local player
void UMainMenuWidget::OnCancelButtonClicked()
{
    if (mbRGameInstance != nullptr)
 	{
 	    //This delegate removes the widget from the viewport, thus it's being reused here
        mbRGameInstance->serverCreation.Broadcast(true);
    }
}

/*
Exits the game when exit button is clicked. This also disconnects the client from the server. If the client is the one
who hosted the server, then the server session is destroyed as well.
*/
void UMainMenuWidget::OnExitButtonClicked()
{	
	if (mbRGameInstance != nullptr)
	{
		mbRGameInstance->EndServer();
	}
	UKismetSystemLibrary::QuitGame(world, playerController, EQuitPreference::Quit, true);
}

//Delegate Function to set the isServerEnded
void UMainMenuWidget::OnServerEnded(bool ended)
{
	isServerEnded = ended;
	if(isServerEnded)
	{
		OnBackButtonClicked();
	}
}


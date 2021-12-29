#include "MainMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Components/EditableText.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime/Core/Public/Windows/COMPointer.h>

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	initialSearchForServers = false;
	customServerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCustomServerButtonClicked);
	serversListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnServersListButtonClicked);
	refreshServersButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnRefreshServersButtonClicked);
	customHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostCustomServerButtonClicked);
	backButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	customServerBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FScriptDelegate serversListDelegate;
	serversListDelegate.BindUFunction(this, "CreateServerSlotWidget");

	if (mbRGameInstance != nullptr)
	{
		mbRGameInstance->serversListDel.Add(serversListDelegate);
	}

	return true;
}

void UMainMenuWidget::OnCustomServerButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(2);
}

void UMainMenuWidget::OnServersListButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(1);
	if (!initialSearchForServers)
	{
		OnRefreshServersButtonClicked();
		initialSearchForServers = true;
	}
}

void UMainMenuWidget::OnRefreshServersButtonClicked()
{
	serverListScrollBox->ClearChildren();
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->FindServers();
	}
}

void UMainMenuWidget::OnHostCustomServerButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->CreateServer(serverNameTextBox->GetText().ToString(), hostNameTextBox->GetText().ToString());
	}
}

void UMainMenuWidget::CreateServerSlotWidget(FServerInfo serverInfo)
{
	if (serverSlotWidget != nullptr)
	{
		UUserWidget* serverSlotUserWidget = CreateWidget<UUserWidget>(GetWorld(), serverSlotWidget);
		if (serverSlotUserWidget != nullptr)
		{
			Cast<UServerSlotWidget>(serverSlotUserWidget)->OnServerInfoUpdate(serverInfo);
			serverListScrollBox->AddChild(serverSlotUserWidget);
		}
	}
}

void UMainMenuWidget::OnBackButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnExitButtonClicked()
{	
	//mbRGameInstance->DestroySession();
	APlayerController* SpecificPlayer = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}


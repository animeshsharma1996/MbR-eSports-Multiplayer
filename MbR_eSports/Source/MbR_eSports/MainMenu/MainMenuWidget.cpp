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

	customServerButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreateServerButtonClicked);
	serversListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnServersListButtonClicked);
	refreshServersButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnRefreshServersButtonClicked);
	customHostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreateCustomServerButtonClicked);
	backButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	customServerBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FScriptDelegate serverDelegate;
	serverDelegate.BindUFunction(this, "CreateServerSlotWidget");

	if (mbRGameInstance != nullptr)
	{
		mbRGameInstance->serversListDel.Add(serverDelegate);
	}

	return true;
}

void UMainMenuWidget::OnCreateServerButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(2);
}

void UMainMenuWidget::OnServersListButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(1);
	OnRefreshServersButtonClicked();
}

void UMainMenuWidget::OnRefreshServersButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->FindServers();
	}
}

void UMainMenuWidget::OnCreateCustomServerButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->CreateServer(serverNameTextBox->GetText().ToString(), hostNameTextBox->GetText().ToString());

		this->RemoveFromViewport();
	}
}

void UMainMenuWidget::CreateServerSlotWidget(FServerInfo serverInfo)
{
	if (serverSlotWidget != nullptr)
	{
		UUserWidget* serverSlotUserWidget = CreateWidget<UUserWidget>(GetWorld(), serverSlotWidget);
		if (serverSlotUserWidget != nullptr)
		{
			UServerSlotWidget* serverSlotWidgetInstance = Cast<UServerSlotWidget>(serverSlotUserWidget);
			serverSlotWidgetInstance->OnServerInfoUpdate(serverInfo);
			serverListScrollBox->AddChild(serverSlotUserWidget);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("FOUND SERVER"));
			}
		}
	}
}

void UMainMenuWidget::OnBackButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnExitButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		//mbRGameInstance->DestroySession();
	}
	APlayerController* SpecificPlayer = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}


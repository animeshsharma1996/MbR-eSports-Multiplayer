#include "MainMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/PanelWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	hostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostButtonClicked);
	serversListButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnServersListButtonClicked);
	refreshServersButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnRefreshServersButtonClicked);
	backButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);
	mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	return true;
}

void UMainMenuWidget::OnHostButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->CreateServer();

		this->RemoveFromViewport();
	}
}

void UMainMenuWidget::OnServersListButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::OnRefreshServersButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->JoinServer();
		CreateServerSlotWidget();
	}
}

void UMainMenuWidget::CreateServerSlotWidget()
{
	if (serverSlotWidget != nullptr)
	{
		UUserWidget* serverSlotUserWidget = CreateWidget<UUserWidget>(GetWorld(), serverSlotWidget);
		if (serverSlotUserWidget != nullptr)
		{
			UServerSlotWidget* serverSlotWidgetInstance = Cast<UServerSlotWidget>(serverSlotUserWidget);
			serverSlotWidgetInstance->OnServerInfoUpdate(mbRGameInstance->serverInfoRecieved);
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


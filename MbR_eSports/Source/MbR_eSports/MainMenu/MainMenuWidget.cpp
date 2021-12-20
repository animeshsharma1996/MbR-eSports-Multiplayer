#include "MainMenuWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	hostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostButtonClicked);
	connectButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnServersListButtonClicked);
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
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->JoinServer();

		this->RemoveFromViewport();
	}
}

void UMainMenuWidget::OnRefreshServersButtonClicked()
{
	widgetSwitcherServerList->SetActiveWidgetIndex(1);
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


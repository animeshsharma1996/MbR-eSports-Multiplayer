#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MbR_eSports.h"

void AMbR_eSportsGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    if (mainMenuWidget != nullptr)
    {
        ChangeMenuWidget(mainMenuWidget);

        UMainMenuWidget* mainMenuWidgetInstance = Cast<UMainMenuWidget>(currentWidget);
        if (serverSlotWidget != nullptr)
        {
            mainMenuWidgetInstance->SetServerSlotWidget(serverSlotWidget);
        }
    }
}

void AMbR_eSportsGameModeBase::ChangeMenuWidget(TSubclassOf<UUserWidget> newWidget)
{
    if (currentWidget != nullptr)
    {
        currentWidget->RemoveFromViewport();
        currentWidget = nullptr;
    }

    if (mainMenuWidget != nullptr)
    {
        currentWidget = Cast<UMainMenuWidget>(CreateWidget<UUserWidget>(GetWorld(), newWidget));
        if (currentWidget != nullptr)
        {
            currentWidget->AddToViewport();

            APlayerController* playerController = GetWorld()->GetFirstPlayerController();
            if (playerController != nullptr)
            {
                playerController->bShowMouseCursor = true;
                playerController->bEnableClickEvents = true;
                playerController->bEnableMouseOverEvents = true;
            }
        }

    }

}
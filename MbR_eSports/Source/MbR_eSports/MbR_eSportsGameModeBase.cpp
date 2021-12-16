#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MbR_eSports.h"

void AMbR_eSportsGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    ChangeMenuWidget(mainMenuWidget);
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
        }
    }

    APlayerController* playerController = GetWorld()->GetFirstPlayerController();;
    playerController->bShowMouseCursor = true;
    playerController->bEnableClickEvents = true;
    playerController->bEnableMouseOverEvents = true;
    playerController->SetInputMode(FInputModeGameOnly());
}
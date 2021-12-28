#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MbR_eSports.h"

void AMbR_eSportsGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (playerController != nullptr)
    {
        playerController->EnableInput(playerController);
        playerController->bShowMouseCursor = true;
        playerController->bEnableClickEvents = true;
        playerController->bEnableMouseOverEvents = true;

        playerController->SetInputMode(InputModeData);
    }

    if (GetWorld()->GetMapName() != "DefaultTestMap")
    {
        CreateMainMenuWidget(mainMenuWidget);
    }

    UMbRGameInstance* mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    FScriptDelegate serverCreationDel;
    serverCreationDel.BindUFunction(this, "RemoveMainMenuFromViewport");

    if (mbRGameInstance != nullptr)
    {
        mbRGameInstance->serverCreation.Add(serverCreationDel);
    }
}

void AMbR_eSportsGameModeBase::CreateMainMenuWidget(TSubclassOf<UUserWidget> newWidget)
{
    if (currentWidget == nullptr)
    {
        if (mainMenuWidget != nullptr)
        {
            currentWidget = Cast<UMainMenuWidget>(CreateWidget<UUserWidget>(GetWorld(), newWidget));
            if (currentWidget != nullptr)
            {
                currentWidget->AddToViewport();
            }
        }
    }
    else
    {
        currentWidget->RemoveFromViewport();
        currentWidget = nullptr;
    }
}

void AMbR_eSportsGameModeBase::RemoveMainMenuFromViewport(bool successful)
{
    UE_LOG(LogTemp, Warning, TEXT("REMOVE FROM VIEWPORT"), successful);

    if (currentWidget != nullptr)
    {
        currentWidget->RemoveFromParent();
        currentWidget = nullptr;
    }
}
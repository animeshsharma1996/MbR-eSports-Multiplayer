#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MbR_eSports.h"

void AMbR_eSportsGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    ChangeMenuWidget(mainMenuWidget);
    if (mbRGameInstance != nullptr)
    {
        mbRGameInstance->SetServerSlotWidget(serverSlotWidget);
    }
}

void AMbR_eSportsGameModeBase::Tick(float deltaSeconds)
{
    Super::Tick(deltaSeconds);
    /*if (GetWorld()->GetCurrentLevel()->GetFName() != "StartingEmptyLevel")
    {
        UE_LOG(LogTemp, Warning, TEXT("PLAYING"));
        DefaultPawnClass = thirdPersonCharacterPawn;
    }
    else
    {
        DefaultPawnClass = mainMenuPawn;
    }*/
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
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

    playerNameText = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->FindComponentByClass<UTextRenderComponent>();

    if (playerNameText != nullptr)
    {
        //playerNameText->SetText(FText::FromString(IOnlineSubsystem::Get()->GetIdentityInterface()->GetPlayerNickname(0)));
    }

 /*   if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, playerNameText->Text.ToString());
    }*/

    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (playerController != nullptr)
    {
        playerController->bShowMouseCursor = true;
        playerController->bEnableClickEvents = true;
        playerController->bEnableMouseOverEvents = true;
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
        mbRGameInstance->SetPlayerCharacter(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
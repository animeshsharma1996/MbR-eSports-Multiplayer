#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MbR_eSports.h"

//Initialize (or begin) events and game elements
void AMbR_eSportsGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    /*
    Logic to enable mouse cursor and mouse events -> Should be extended initialize player controller variable
    for other classes as well
    */
    APlayerController* playerController = GetWorld()->GetFirstPlayerController();
    if (playerController != nullptr)
    {
        playerController->bShowMouseCursor = true;
        playerController->bEnableClickEvents = true;
        playerController->bEnableMouseOverEvents = true;
    }
    
    //Create the main Menu widget when in the main menu only -> Might have to change accordingly
    if (GetWorld()->GetMapName() != defaultGameMapName)
    {
        CreateMainMenuWidget(mainMenuWidget);
    }
    else //In-Game Menu
    {
        
    }

    //Logic to bind server creation delegate function -> Fires the RemoveMainMenuFromViewport upon creation
    UMbRGameInstance* mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    FScriptDelegate serverCreationDel;
    serverCreationDel.BindUFunction(this, "RemoveMainMenuFromViewport");
    if (mbRGameInstance != nullptr)
    {
        mbRGameInstance->serverCreation.Add(serverCreationDel);
    }
}

//Create the Main Menu Widget and add it to the viewport upon start of the game
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

/*
Function required to remove the Main Menu Widget from Viewport when the server is started (map is loaded) or when 
another player joins the server
*/
void AMbR_eSportsGameModeBase::RemoveMainMenuFromViewport(bool successful)
{
    UE_LOG(LogTemp, Warning, TEXT("REMOVE FROM VIEWPORT"), successful);

    if (currentWidget != nullptr)
    {
        currentWidget->RemoveFromParent();
        currentWidget = nullptr;
    }
}
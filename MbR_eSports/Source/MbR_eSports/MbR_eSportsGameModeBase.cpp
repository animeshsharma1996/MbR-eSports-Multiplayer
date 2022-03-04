#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MbR_eSports.h"

//Constructor for GameModeBase
AMbR_eSportsGameModeBase::AMbR_eSportsGameModeBase()
{
    //Enabling the Tick function for this game mode base class
    PrimaryActorTick.bCanEverTick = true;
}

//Initialize (or begin) events and game elements
void AMbR_eSportsGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    //Set a reference for the Main Menu variable
    if (mainMenuWidget != nullptr)
    {
         mainMenuUserWidget = Cast<UMainMenuWidget>(CreateWidget<UUserWidget>(GetWorld(), mainMenuWidget));
    }
    
    /*
    Logic to enable mouse cursor and mouse events -> Should be extended to initialize player controller variable
    for other classes as well
    */
    playerController = GetWorld()->GetFirstPlayerController();
    if (playerController != nullptr)
    {
        playerController->bShowMouseCursor = true;
        playerController->bEnableClickEvents = true;
        playerController->bEnableMouseOverEvents = true;
    }
    
    //Create the main Menu widget when in the main menu only -> Might have to change accordingly
    if (GetWorld()->GetMapName() != defaultGameMapName)
    {
        CreateMainMenuWidget();
    }

    //Logic to bind server creation delegate function -> Fires the RemoveMainMenuFromViewport upon creation
    UMbRGameInstance* mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    FScriptDelegate serverCreationDel;
    serverCreationDel.BindUFunction(this, "RemoveMainMenuFromViewport");
    
    if (mbRGameInstance != nullptr)
    {
        mbRGameInstance->serverCreation.Add(serverCreationDel);
        mbRGameInstance->AssignMapNames(FName(defaultGameMapName), FName(mainMenuMapName));
    }
}

//Manifest events during gameplay
void AMbR_eSportsGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    BringUpInGameMenu();

    if (GetWorld()->GetMapName() != defaultGameMapName)
    {
        if (mainMenuUserWidget != nullptr && currentWidget != nullptr)
        {
            Cast<UMainMenuWidget>(currentWidget)->PublicTick();
        }
    }
}

//Create the Main Menu Widget and add it to the viewport upon start of the game
void AMbR_eSportsGameModeBase::CreateMainMenuWidget()
{
    if (currentWidget == nullptr)
    {
        currentWidget = mainMenuUserWidget;
        currentWidget->AddToViewport();
    }
    else
    {
        currentWidget->RemoveFromViewport();
        currentWidget = nullptr;
    }
}

//The in-game menu should bring up in any map except for main menu (will only happen if there is a player controller)
void AMbR_eSportsGameModeBase::BringUpInGameMenu()
{
    if (GetWorld()->GetMapName() != mainMenuMapName && playerController != nullptr)
    {
        //Press escape to bring up the In game menu (possible only when mainMenuUserWidget is not null)
        if (playerController->IsInputKeyDown(EKeys::Escape) && mainMenuUserWidget != nullptr)
        {
            if (currentWidget == nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("In-GameMenu Locally"));
                currentWidget = mainMenuUserWidget;
                Cast<UMainMenuWidget>(currentWidget)->InGameMenu();
                currentWidget->AddToViewport();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Remove the In-GameMenu Locally"));
                currentWidget->RemoveFromViewport();
                currentWidget = nullptr;
            }
        }
    }
}

/*
Function required to remove the Main Menu Widget from Viewport when the server is started (map is loaded) or when 
another player (for that particular player) joins the server.
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
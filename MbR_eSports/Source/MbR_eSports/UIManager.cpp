// Fill out your copyright notice in the Description page of Project Settings.

#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenuWidget.h"
#include "ChatSystem/ChatWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "MbR_eSports.h"

void AUIManager::BeginPlay()
{
    Super::BeginPlay();

    isInGameMenuUp = false;
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

    if (GetWorld())
    {
        world = GetWorld();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No World Found"));
        return;
    }

    mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(world));
    playerController = world->GetFirstPlayerController();

    //Set a reference for the Main Menu variable
    if (mainMenuWidget != nullptr)
    {
         mainMenuUserWidget = Cast<UMainMenuWidget>(CreateWidget<UUserWidget>(world, mainMenuWidget));
    }
    
    //Logic to bind server creation delegate function -> Fires the RemoveMainMenuFromViewport upon creation
    FScriptDelegate serverCreationDel;
    serverCreationDel.BindUFunction(this, "RemoveMainMenuFromViewport");

    //Create the main Menu widget when in the main menu only -> Might have to change accordingly
    if (world->GetMapName() != defaultGameMapName)
    {
        CreateMainMenuWidget();
    }
    else
    {
        //Spawn the replicated RPC actor in the level
        if (rpcActorClass != nullptr)
        {
            rPCActor = Cast<ARPCActor>(world->SpawnActor(rpcActorClass));
        }

        //Set a reference for the Chat Widget variable
        if(chatWidget != nullptr)
        {
            chatUserWidget = Cast<UChatWidget>(CreateWidget<UUserWidget>(world, chatWidget));
        }

        //Add the chat widget in the viewport
        if(chatUserWidget != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Chat Added"));
            chatUserWidget->AddToViewport();
        }
    }

    //Assinging the Game Instance variables and adding server creation delegates
    if (mbRGameInstance != nullptr)
    {
        mbRGameInstance->serverCreation.Add(serverCreationDel);
        if (playerController != nullptr)
        {
            //Logic to enable mouse cursor and mouse events
            playerController->bShowMouseCursor = true;
            playerController->bEnableClickEvents = true;
            playerController->bEnableMouseOverEvents = true;

            mbRGameInstance->SetAssignables(FName(defaultGameMapName), FName(mainMenuMapName), playerController, world);
        }

        //Initialise the replicated RPC actor
        if (rPCActor != nullptr)
        {
            rPCActor->Initialise(mbRGameInstance);
        }
    }
}

//Create the Main Menu Widget and add it to the viewport upon start of the game
void AUIManager::CreateMainMenuWidget()
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
void AUIManager::BringUpInGameMenu()
{
    if (world && world->GetMapName() != mainMenuMapName)
    {
        //Press escape to bring up the In game menu (possible only when mainMenuUserWidget is not null)
        if (mainMenuUserWidget != nullptr && !isInGameMenuUp)
        {
            if (currentWidget == nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("In-GameMenu Locally"));
                currentWidget = mainMenuUserWidget;
                Cast<UMainMenuWidget>(currentWidget)->InGameMenu();
                currentWidget->AddToViewport();
                isInGameMenuUp = true;
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
void AUIManager::RemoveMainMenuFromViewport(bool successful)
{
    UE_LOG(LogTemp, Warning, TEXT("REMOVE FROM VIEWPORT"), successful);

    if (currentWidget != nullptr)
    {
        currentWidget->RemoveFromParent();
        currentWidget = nullptr;
        isInGameMenuUp = false;
    }
}
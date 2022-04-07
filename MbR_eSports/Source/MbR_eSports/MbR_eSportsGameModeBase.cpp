#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu/MainMenuWidget.h"
#include "UIManager.h"
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
    
    playerController = GetWorld()->GetFirstPlayerController();
    if(playerController != nullptr)
    {
        uIManager = Cast<AUIManager>(playerController->GetHUD());
        if(uIManager != nullptr && GetWorld())
        {
            uIManager->Initialise(playerController, GetWorld());
        }
    }
}

//Manifest events during gameplay
void AMbR_eSportsGameModeBase::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   if(uIManager != nullptr)
   {
        uIManager->OnTick(DeltaTime);
   }
}
#include "MbR_eSportsGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "MbR_eSports.h"

//Constructor for GameModeBase
AMbR_eSportsGameModeBase::AMbR_eSportsGameModeBase()
{
    //Enabling the Tick function for this game mode base class
    PrimaryActorTick.bCanEverTick = true;
}

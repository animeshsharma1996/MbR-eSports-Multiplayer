#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu/MainMenuWidget.h"
#include "Net/UnrealNetwork.h"
#include "MbR_eSportsGameModeBase.generated.h"

/*
The GameModeBase class is being derived from AGameModeBase. Generally meant to define the rules of the game 
(Should be extended to accomplish that task).
Here the class is utilised to initialise and start the UI Manager (derived from HUD class)
*/

UCLASS()
class MBR_ESPORTS_API AMbR_eSportsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
        AMbR_eSportsGameModeBase();
};

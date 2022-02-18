#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu/MainMenuWidget.h"
#include "Net/UnrealNetwork.h"
#include "MbR_eSportsGameModeBase.generated.h"

/*
The GameModeBase class is being derived from AGameModeBase. Generally meant to define the rules of the game 
(Should be extended to accomplish that task).
Here the class is utilised to initialise and start the widgets (MainMenu, ServerSlot) and the GameInstance.
The variables and functions names are self explanatory.
*/

UCLASS()
class MBR_ESPORTS_API AMbR_eSportsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
    AMbR_eSportsGameModeBase();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        TSubclassOf<UUserWidget> mainMenuWidget;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        TSubclassOf<UUserWidget> serverSlotWidget;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        FString defaultGameMapName;    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        FString mainMenuMapName;
    UFUNCTION(BlueprintCallable)
        void CreateMainMenuWidget();

private:
    UPROPERTY()
        UUserWidget* currentWidget; 
    UPROPERTY()
        UUserWidget* mainMenuUserWidget;   
    UPROPERTY()   
        APlayerController* playerController;
    UFUNCTION()
        void RemoveMainMenuFromViewport(bool successful);
};

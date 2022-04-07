// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UIManager.generated.h"

/**
Here the class is utilised to initialise and start the widgets (MainMenu, ServerSlot) and the GameInstance.
The variables and functions names are self explanatory.
 **/
UCLASS()
class MBR_ESPORTS_API AUIManager : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void Initialise(APlayerController* playerController, UWorld* world);
	UFUNCTION(BlueprintCallable)
		void OnTick(float DeltaTime);	

protected:
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
    UFUNCTION(BlueprintCallable)
        void BringUpInGameMenu();

private:
    UPROPERTY()
        UUserWidget* currentWidget; 
    UPROPERTY()
        UUserWidget* mainMenuUserWidget;   
    UPROPERTY()   
        APlayerController* playerController;
    UPROPERTY() 
        UWorld* world;
    UPROPERTY()
        bool isInGameMenuUp;

    UFUNCTION()
        void RemoveMainMenuFromViewport(bool successful);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MbRGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "RPCActor.h"
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
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;	

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<UUserWidget> mainMenuWidget;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<UUserWidget> serverSlotWidget;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<AActor> rpcActorClass;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        FString defaultGameMapName;    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
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
        UMbRGameInstance* mbRGameInstance;
    UPROPERTY()
        ARPCActor* rPCActor;
    UPROPERTY()
        bool isInGameMenuUp;

    UFUNCTION()
        void RemoveMainMenuFromViewport(bool successful);
};

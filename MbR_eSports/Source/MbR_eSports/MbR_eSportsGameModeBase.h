#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu/MainMenuWidget.h"
#include "MbR_eSportsGameModeBase.generated.h"

UCLASS()
class MBR_ESPORTS_API AMbR_eSportsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        TSubclassOf<UUserWidget> mainMenuWidget;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        TSubclassOf<UUserWidget> serverSlotWidget;
    UFUNCTION(BlueprintCallable)
        void CreateMainMenuWidget(TSubclassOf<UUserWidget> newWidget);

private:
    UPROPERTY()
        UUserWidget* currentWidget;
    UFUNCTION()
        void RemoveMainMenuFromViewport(bool successful);
};

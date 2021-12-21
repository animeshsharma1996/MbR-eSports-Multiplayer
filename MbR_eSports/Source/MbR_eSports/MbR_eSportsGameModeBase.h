#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenu/MainMenuWidget.h"
#include "MbR_eSportsGameModeBase.generated.h"

UCLASS()
class MBR_ESPORTS_API AMbR_eSportsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "MbR eSports")
        void ChangeMenuWidget(TSubclassOf<UUserWidget> newWidget);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        TSubclassOf<UUserWidget> mainMenuWidget;    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MbR_eSports")
        TSubclassOf<UUserWidget> serverSlotWidget;
    UPROPERTY(EditAnywhere, Category = "MbR_eSports")
        TSubclassOf<APawn> thirdPersonCharacterPawn;    
    UPROPERTY(EditAnywhere, Category = "MbR_eSports")
        TSubclassOf<APawn> mainMenuPawn;


protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltaSeconds) override;

    UPROPERTY()
        UUserWidget* currentWidget;
};

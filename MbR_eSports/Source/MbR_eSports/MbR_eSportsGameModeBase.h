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

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
        UUserWidget* currentWidget;
};

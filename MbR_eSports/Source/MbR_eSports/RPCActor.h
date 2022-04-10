// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MbRGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "RPCActor.generated.h"

UCLASS()
class MBR_ESPORTS_API ARPCActor : public AActor
{
    GENERATED_BODY()

public:
    UFUNCTION()
        void Initialise(UMbRGameInstance* gameInstance, UWorld* uWorld);
    UFUNCTION(BlueprintCallable)
        void HandleEndSession(bool successful);
    UFUNCTION(BlueprintCallable)
        UMbRGameInstance* GetGameInstance() { return mbRGameInstance; }

private:
    UPROPERTY()
        UWorld* world;
    UPROPERTY()
        UMbRGameInstance* mbRGameInstance;

    UFUNCTION(NetMulticast, Reliable)
        void ClientOnEndSession();

};

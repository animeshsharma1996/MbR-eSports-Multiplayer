// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MbRGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "RPCActor.generated.h"

//RPC Actor class to have atleast one replicated actor in the level owned by clients
UCLASS()
class MBR_ESPORTS_API ARPCActor : public AActor
{
    GENERATED_BODY()
        ARPCActor(const class FObjectInitializer& PCIP);

public:
    UFUNCTION()
        void Initialise(UMbRGameInstance* gameInstance);
    UFUNCTION(BlueprintCallable)
        void HandleEndSession(bool successful);
    UFUNCTION(BlueprintCallable)
        UMbRGameInstance* GetGameInstance() { return mbRGameInstance; }
    UFUNCTION(Server, Reliable)
        void RegisterPlayer(FName sessionName, const FUniqueNetIdRepl playerId, bool bWasInvited);

private:
    UPROPERTY()
        UMbRGameInstance* mbRGameInstance;
    UFUNCTION(NetMulticast, Reliable)
        void ClientOnEndSession();

};

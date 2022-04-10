// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MbRGameInstance.h"

void ARPCActor::Initialise(UMbRGameInstance* gameInstance, UWorld* uWorld)
{
    UE_LOG(LogTemp, Warning, TEXT("RPC Actor Initialisation"));

    FScriptDelegate serverEndDel;
    serverEndDel.BindUFunction(this, "HandleEndSession");
    world = uWorld;
    mbRGameInstance = gameInstance;
    mbRGameInstance->endServerDel.Add(serverEndDel);
}

void ARPCActor::HandleEndSession(bool successful)
{
    UE_LOG(LogTemp, Warning, TEXT("Initiate End Session"));

    ClientOnEndSession();
}

void ARPCActor::ClientOnEndSession_Implementation()
{
    Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->OnEndServer();
    if (mbRGameInstance != nullptr)
    {
        //mbRGameInstance->OnEndServer();
    }
}
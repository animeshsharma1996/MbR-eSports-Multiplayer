// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MbRGameInstance.h"

//Initialise the Replicated RPC Actor and assign the delegates to the game Instance
void ARPCActor::Initialise(UMbRGameInstance* gameInstance)
{
    UE_LOG(LogTemp, Warning, TEXT("RPC Actor Initialisation"));

    FScriptDelegate serverEndDel;
    serverEndDel.BindUFunction(this, "HandleEndSession");
    mbRGameInstance = gameInstance;
    mbRGameInstance->endServerDel.Add(serverEndDel);
}

//Delegate function fired when any player tries to leave the game
void ARPCActor::HandleEndSession(bool successful)
{
    ClientOnEndSession();
}

/*RPC function -> If the host leaves, the RPC is called on each client. If the connected played leaves, the RPC is 
called on the client (that particular connected player) only*/
void ARPCActor::ClientOnEndSession_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Initiate End Session"));
    Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->OnEndServer();
}
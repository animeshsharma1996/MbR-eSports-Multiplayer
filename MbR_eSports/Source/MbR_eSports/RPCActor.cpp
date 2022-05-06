// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "MbRGameInstance.h"
#include "MbR_eSportsGameModeBase.h"

ARPCActor::ARPCActor(const class FObjectInitializer& PCIP) : Super(PCIP)
{
    bReplicates = true;
}

//Initialise the Replicated RPC Actor and assign the delegates to the game Instance
void ARPCActor::Initialise(UMbRGameInstance* gameInstance)
{
    FScriptDelegate serverEndDel;
    FScriptDelegate registerPlayerDel;
    serverEndDel.BindUFunction(this, "HandleEndSession");
    registerPlayerDel.BindUFunction(this, "RegisterPlayer");
    mbRGameInstance = gameInstance;
    mbRGameInstance->endServerDel.Add(serverEndDel);
    mbRGameInstance->registerPlayersDel.Add(registerPlayerDel);
}

//Delegate function fired when any player tries to leave the game
void ARPCActor::HandleEndSession(bool successful)
{
    ClientOnEndSession();
}

//Function runnning on the server to register the player in the session
void ARPCActor::RegisterPlayer_Implementation(FName sessionName, const FUniqueNetIdRepl playerId, bool bWasInvited)
{
    Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->RegisterPlayer(sessionName, playerId, bWasInvited);
}

/*RPC function -> If the host leaves, the RPC is called on each client. If the connected played leaves, the RPC is 
called on the client (that particular connected player) only*/
void ARPCActor::ClientOnEndSession_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Initiate End Session"));
    Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->OnEndServer();
}
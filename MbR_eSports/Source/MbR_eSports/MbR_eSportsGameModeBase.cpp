#include "MbR_eSportsGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/World.h"
#include "OnlineSubsystem.h"
#include "MbR_eSports.h"

//Constructor for GameModeBase
AMbR_eSportsGameModeBase::AMbR_eSportsGameModeBase()
{
    //Enabling the Tick function for this game mode base class
    PrimaryActorTick.bCanEverTick = true;
    sessionName = FName("Empty Session");
}

void AMbR_eSportsGameModeBase::PostLogin(APlayerController* InPlayerController)
{
    check(IsValid(InPlayerController));

    // This code handles logins for both the local player (listen server) and remote players (net connection).
    FUniqueNetIdRepl UniqueNetIdRepl;
    if (InPlayerController->IsLocalPlayerController())
    {
        ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
        if (IsValid(LocalPlayer))
        {
            UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
        }
        else
        {
            UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
            check(IsValid(RemoteNetConnection));
            UniqueNetIdRepl = RemoteNetConnection->PlayerId;
        }
    }
    else
    {
        UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
        check(IsValid(RemoteNetConnection));
        UniqueNetIdRepl = RemoteNetConnection->PlayerId;
    }

    // Get the unique player ID.
    TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
    check(UniqueNetId != nullptr);

    // Get the online session interface.
    IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();  
    IOnlineSessionPtr session = subsystem->GetSessionInterface();
    
    if (mbRGameInstance == nullptr)
    {
        mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        sessionName = mbRGameInstance->GetSessionName();
    }

    // Register the player with the "sessionName" session; this name should match the name provided in CreateSession.
    if (!session->RegisterPlayer(sessionName, *UniqueNetId.Get(), false))
    {
        // The player could not be registered; typically will want to kick the player from the server in this situation.
        UE_LOG(LogTemp, Warning, TEXT("Player couldn't be registered"));
    }

    Super::PostLogin(InPlayerController);
}
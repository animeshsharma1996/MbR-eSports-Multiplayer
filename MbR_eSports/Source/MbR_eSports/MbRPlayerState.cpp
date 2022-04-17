// Fill out your copyright notice in the Description page of Project Settings.


#include "MbRPlayerState.h"
#include "Engine/World.h"

AMbRPlayerState::AMbRPlayerState(const class FObjectInitializer& PCIP) : Super(PCIP)
{
    bReplicates = true;
}

void AMbRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMbRPlayerState, chatWidget);
    DOREPLIFETIME(AMbRPlayerState, chatMessage);
}

void AMbRPlayerState::ClientInitialize(AController* InController)
{
    Super::ClientInitialize(InController);
}

void AMbRPlayerState::BeginPlay()
{
    if (GetWorld()->GetMapName() != "MainMenu")
    {
        CreateChatWidget();
    }
}


void AMbRPlayerState::SetWidget()
{
    SetWidgetServer(chatWidget);
    FScriptDelegate messageSendDelegate;
    messageSendDelegate.BindUFunction(this, "SendChatMessageToServer");
    chatWidget->messageSendDel.Add(messageSendDelegate);
}

void AMbRPlayerState::SetWidgetServer_Implementation(UChatWidget* widget)
{
    chatWidget = widget;
}

//Create the chat widget function RPC
void AMbRPlayerState::CreateChatWidget()
{
    //Set a reference for the Chat Widget variable
    if (chatWidgetClass != nullptr)
    {
        chatWidget = Cast<UChatWidget>(CreateWidget<UUserWidget>(GetWorld(), chatWidgetClass));
    }

    //Add the chat widget in the viewport
    if (chatWidget != nullptr)
    {
        chatWidget->AddToViewport();
        SetWidget();
    }
}

void AMbRPlayerState::SendChatMessageToServer_Implementation(const FString& message)
{
    chatMessage = *message;
    SendMessageToAll(chatMessage);
}

void AMbRPlayerState::SendMessageToAll_Implementation(const FString& message)
{
    UE_LOG(LogTemp, Warning, TEXT("Sent Message To All Clients %s"), *message);

    if (chatWidget != nullptr)
    {
        chatWidget->OnChatMessageTypedToAll(message);
    }
}
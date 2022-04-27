// Fill out your copyright notice in the Description page of Project Settings.


#include "MbRPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Engine/World.h"

AMbRPlayerController::AMbRPlayerController(const class FObjectInitializer& PCIP) : Super(PCIP)
{
    bReplicates = true;
}

void AMbRPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMbRPlayerController, chatWidget);
    DOREPLIFETIME(AMbRPlayerController, chatMessage);
}

void AMbRPlayerController::BeginPlay()
{
    if (GetWorld()->GetMapName() != "MainMenu")
    {
        CreateChatWidget();
    }
}

//Create the chat widget function RPC
void AMbRPlayerController::CreateChatWidget()
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

void AMbRPlayerController::SetWidget()
{
    SetWidgetServer(chatWidget);
    FScriptDelegate messageSendDelegate;
    messageSendDelegate.BindUFunction(this, "SendChatMessageToServer");
    chatWidget->messageSendDel.Add(messageSendDelegate);
}

void AMbRPlayerController::SetWidgetServer_Implementation(UChatWidget* widget)
{
    chatWidget = widget;
}

void AMbRPlayerController::SendChatMessageToServer_Implementation(const FString& message)
{
    chatMessage = *message;
    SendMessageToAll(chatMessage);
}

void AMbRPlayerController::SendMessageToAll_Implementation(const FString& message)
{
    UE_LOG(LogTemp, Warning, TEXT("Sent Message To All Clients %s"), *message);

    if (chatWidget != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("huh?"));

        chatWidget->OnChatMessageTypedToAll(message);
    }
}

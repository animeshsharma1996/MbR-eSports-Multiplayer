// Fill out your copyright notice in the Description page of Project Settings.


#include "MbRPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"

void AMbRPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("BringChat", IE_Pressed, this, &AMbRPlayerController::BringUpChat);
}

void AMbRPlayerController::BeginPlay()
{
    if (GetWorld()->GetMapName() != "MainMenu")
    {
        CreateChatWidget();
    }
}

//Create the chat widget function RPC
void AMbRPlayerController::CreateChatWidget_Implementation()
{
    //Set a reference for the Chat Widget variable
    if (chatWidgetClass != nullptr && chatWidget == nullptr)
    {
        if (Cast<UChatWidget>(CreateWidget<UUserWidget>(GetWorld(), chatWidgetClass)))
        {
            UChatWidget* createdWidget = Cast<UChatWidget>(CreateWidget<UUserWidget>(GetWorld(), chatWidgetClass));
            chatWidget = createdWidget;
            chatWidget->AddToViewport();
            chatWidget->SetPlayerName("PC");
            if (PlayerState != nullptr)
            {
                chatWidget->SetPlayerName(PlayerState->GetPlayerName());
            }
            SetWidget();
        }
    }
}

void AMbRPlayerController::SetWidget()
{
    FScriptDelegate messageSendDelegate;
    messageSendDelegate.BindUFunction(this, "SendChatMessageToServer");
    chatWidget->messageSendDel.Add(messageSendDelegate);
}

void AMbRPlayerController::BringUpChat()
{
    chatWidget->SetVisibility(ESlateVisibility::Visible);
    chatWidget->SetKeyboardFocusOnText();
}

void AMbRPlayerController::SendChatMessageToServer_Implementation(const FString& message)
{
    for (FConstPlayerControllerIterator pC = GetWorld()->GetPlayerControllerIterator(); pC; ++pC)
    {
        AMbRPlayerController* pController = Cast<AMbRPlayerController>(*pC);
        pController->SendMessageToAll(message);
    }
}

void AMbRPlayerController::SendMessageToAll_Implementation(const FString& message)
{
    if (chatWidget != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sent Message To All Clients FN Invoke"));

        chatWidget->OnChatMessageTypedToAll(message);
    }
}

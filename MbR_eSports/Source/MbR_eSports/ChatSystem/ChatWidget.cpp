// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Components/EditableText.h"
#include "Engine/World.h"
#include "Components/ScrollBox.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

void UChatWidget::OnChatMessageTyped()
{
    OnChatMessageTypedToServer();
}

void UChatWidget::OnChatMessageTypedToServer()
{
    FString chatMessage = chatMessageTextBox->GetText().ToString();
    FText chatText = chatMessageTextBox->GetText();

    if (chatMessage.IsEmpty())
    {
        return;
    }
   
    messageSendDel.Broadcast(chatMessage);
    chatMessageTextBox->SetText(FText::AsCultureInvariant(""));

}

void UChatWidget::OnChatMessageTypedToAll(const FString& message)
{
    UE_LOG(LogTemp, Warning, TEXT("Sent Message To All working"));
    UChatMessageWidget* chatTextWidget = nullptr;

    if (chatMessageWidget != nullptr)
    {
        chatTextWidget = Cast<UChatMessageWidget>(CreateWidget<UUserWidget>(GetWorld(), chatMessageWidget));
        if (chatTextWidget != nullptr)
        {
            chatTextWidget->SetChatText(FText::AsCultureInvariant(message));
        }
        else
        {
            return;
        }
    }
    chatMessagesScrollBox->AddChild(chatTextWidget);
}

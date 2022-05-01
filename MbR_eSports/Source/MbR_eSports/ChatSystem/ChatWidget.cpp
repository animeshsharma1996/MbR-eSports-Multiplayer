// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWidget.h"
#include "Components/EditableText.h"
#include "Engine/World.h"
#include "Components/ScrollBox.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

void UChatWidget::NativeConstruct()
{
    Super::NativeConstruct();
    chatMessageTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::OnChatMessageTyped);
}

void UChatWidget::OnChatMessageTyped(const FText& Text, ETextCommit::Type CommitMethod)
{
    if(CommitMethod == ETextCommit::OnEnter)
    {
        FString textString = Text.ToString();
        if (!playerName.IsEmpty() && !textString.IsEmpty())
        {
            FString messageString = playerName + ": " + textString;
            messageSendDel.Broadcast(messageString);
            chatMessageTextBox->SetText(FText::AsCultureInvariant(""));
        }
    }
}

void UChatWidget::OnChatMessageTypedToAll(const FString& message)
{
    UE_LOG(LogTemp, Warning, TEXT("Sent Message To All Clients %s"), *message);
    UChatMessageWidget* chatTextWidget = nullptr;
    if (chatMessageWidget)
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

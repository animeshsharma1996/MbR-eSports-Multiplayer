// Fill out your copyright notice in the Description page of Project Settings.

#include "ChatWidget.h"
#include "Components/EditableText.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/World.h"
#include "Components/ScrollBox.h"
#include "TimerManager.h"

/*Construct the chat widget by setting keyboard focus as default upon commit
* Bind the ChatMessageTyped function with Chat Message Box commit delegate
* Also intialise appropriate variables*/
void UChatWidget::NativeConstruct()
{
    Super::NativeConstruct();
    chatMessageTextBox->SetClearKeyboardFocusOnCommit(false);
    chatMessageTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::OnChatMessageTyped);
    chatTextWidget = nullptr;
    canvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(chatWidgetBorder);
    currentY = canvasPanelSlot->GetSize().Y;
    currentX = canvasPanelSlot->GetSize().X;
}

//Unhide the chat widget -> Called by the player controller when pressed enter
void UChatWidget::UnHideChatWidget()
{
    SetVisibility(ESlateVisibility::Visible);
    SetKeyboardFocusOnText();
    chatMessagesScrollBox->ScrollToEnd();
}

//Send the message as broadcast to the player controller when the message is typed
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

//Add the chat message to the chat box after making it visible on screen
void UChatWidget::AddTheChatMessageToChatBox(const FString& message)
{
    UE_LOG(LogTemp, Warning, TEXT("Sent Message To All Clients %s"), *message);
    //Clear the timer handle and set the chat to visible
    GetWorld()->GetTimerManager().ClearTimer(timerHandle);
    SetVisibility(ESlateVisibility::Visible);

    //Create the child chat Message Widget with the correct message and add it to the scroll box/chat screen
    if (chatMessageWidget)
    {
        chatTextWidget = Cast<UChatMessageWidget>(CreateWidget<UUserWidget>(GetWorld(), chatMessageWidget));
        if (chatTextWidget == nullptr)  { return; }
        chatTextWidget->SetChatText(FText::AsCultureInvariant(message));
    }
    chatMessagesScrollBox->AddChild(chatTextWidget);

    //Increment the size of the chat widget upon adding each message
    if (canvasPanelSlot != nullptr)
    {
        float incrementY = currentY + incrementInY;
        if (incrementY <= maxY)
        {
            currentY = incrementY;
            canvasPanelSlot->SetSize(FVector2D(currentX, incrementY));
        }
    }
    chatMessagesScrollBox->ScrollToEnd();

    //Timer to hide the chat after a delay and reset the size of the chat box along with scrolling to the end
    GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UChatWidget::HideChatWidget, chatHideDelay, false);
}

//Hide the chat, reset the size of the chat box and clear the timer handle
void UChatWidget::HideChatWidget()
{
    SetVisibility(ESlateVisibility::Hidden);
    canvasPanelSlot->SetSize(FVector2D(currentX, minY));
    GetWorld()->GetTimerManager().ClearTimer(timerHandle);
}

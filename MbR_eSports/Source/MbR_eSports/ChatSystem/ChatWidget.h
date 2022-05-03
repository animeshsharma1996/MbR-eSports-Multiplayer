// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/EditableText.h"
#include "Components/Border.h"
#include "Widgets/SWidget.h"
#include "Types/SlateEnums.h"
#include "Net/UnrealNetwork.h"
#include "SlateBasics.h"
#include "ChatMessageWidget.h"
#include "ChatWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSendMessage, const FString&, message);

/**
 * Chat Widget responsible to creating chat box, binding variables and writing/adding chat messages
 */
UCLASS()
class MBR_ESPORTS_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
		FDelegateSendMessage messageSendDel;	

	UFUNCTION(BlueprintCallable)
		void SetPlayerName(FString name) { playerName = name; }
	UFUNCTION(BlueprintCallable)
		FString GetPlayerName() { return playerName; }
	UFUNCTION(BlueprintCallable)
		void SetKeyboardFocusOnText() {	chatMessageTextBox->SetKeyboardFocus();	}
	UFUNCTION(BlueprintCallable)
		void UnHideChatWidget();
	UFUNCTION(BlueprintCallable)
		void OnChatMessageTyped(const FText& Text, const ETextCommit::Type CommitMethod);
	UFUNCTION(BlueprintCallable)
		void AddTheChatMessageToChatBox(const FString& chatMessage);

protected :
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* chatMessagesScrollBox;
	UPROPERTY(meta = (BindWidget))
		class UEditableText* chatMessageTextBox;	
	UPROPERTY(meta = (BindWidget))
		class UBorder* chatWidgetBorder;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UUserWidget> chatMessageWidget;

	UFUNCTION(BlueprintCallable)
		void SetMessageText(FText text) { chatMessageTextBox->SetText(text); }	
	UFUNCTION(BlueprintCallable)
		void HideChatWidget();

private :
	FTimerHandle timerHandle;

	UPROPERTY()
		UChatMessageWidget* chatTextWidget;
	UPROPERTY()
		UCanvasPanelSlot* canvasPanelSlot;
	UPROPERTY()
		FString playerName;
	UPROPERTY()
		float chatHideDelay = 10.0F;
	UPROPERTY()
		float currentY = 0.0F;
	UPROPERTY()
		float currentX = 0.0F;
	UPROPERTY()
		float incrementInY = 40.0F;	
	UPROPERTY()
		float maxY = 370.0F;
	UPROPERTY()
		float minY = 130.0F;

};

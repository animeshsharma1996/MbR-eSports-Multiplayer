// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "ChatMessageWidget.h"
#include "ChatWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSendMessage, const FString&, message);

/**
 * 
 */
UCLASS()
class MBR_ESPORTS_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintAssignable)
		FDelegateSendMessage messageSendDel;	

	UFUNCTION(BlueprintCallable)
		void OnChatMessageTyped();	
	UFUNCTION(BlueprintCallable)
		void OnChatMessageTypedToServer();
	UFUNCTION(BlueprintCallable)
		void OnChatMessageTypedToAll(const FString& chatMessage);

protected :
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* chatMessagesScrollBox;
	UPROPERTY(meta = (BindWidget))
		class UEditableText* chatMessageTextBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UUserWidget> chatMessageWidget;

	UFUNCTION(BlueprintCallable)
		void SetMessageText(FText text) { chatMessageTextBox->SetText(text); }	

private :
	UPROPERTY()
		float maxHeight = 370.0F;
	UPROPERTY()
		float minHeight = 70.0F;

};

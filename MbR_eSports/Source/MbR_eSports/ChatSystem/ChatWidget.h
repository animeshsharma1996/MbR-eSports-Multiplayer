// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "Widgets/SWidget.h"
#include "Types/SlateEnums.h"
#include "Net/UnrealNetwork.h"
#include "SlateBasics.h"
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
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable)
		FDelegateSendMessage messageSendDel;	

	UFUNCTION(BlueprintCallable)
		void SetPlayerName(FString name) { playerName = name; }
	UFUNCTION(BlueprintCallable)
		void OnChatMessageTyped(const FText& Text, const ETextCommit::Type CommitMethod);
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
		FString playerName;
	UPROPERTY()
		float maxHeight = 370.0F;
	UPROPERTY()
		float minHeight = 90.0F;

};

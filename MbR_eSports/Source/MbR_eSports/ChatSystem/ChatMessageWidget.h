// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ChatMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class MBR_ESPORTS_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()
	
public :
	UFUNCTION(BlueprintCallable)
		void SetChatText(FText text) 
		{
			chatMessage->SetText(text); 
		}
	UFUNCTION(BlueprintCallable)
		FText GetChatText() { return chatMessage->GetText(); }

protected :
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* chatMessage;
};

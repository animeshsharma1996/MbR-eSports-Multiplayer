// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

/**
 * 
 */
UCLASS()
class MBR_ESPORTS_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected :
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* chatMessagesTextBlock;
	UPROPERTY(meta = (BindWidget))
		class UMultiLineEditableText* chatMessageTextBox;	

};

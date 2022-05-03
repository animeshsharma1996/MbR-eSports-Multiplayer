// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatSystem/ChatWidget.h"
#include "Net/UnrealNetwork.h"
#include "ChatSystem/ChatMessageWidget.h"
#include "GameFramework/PlayerController.h"
#include "MbRPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSentMessage, const FString&, chatString);

/* 
Player Controller class to recieve input, create chat widget and own it for the purpose of runnning RPCs
*/
UCLASS()
class MBR_ESPORTS_API AMbRPlayerController : public APlayerController
{
	GENERATED_BODY()
        virtual void BeginPlay() override;
        virtual void SetupInputComponent() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<UUserWidget> chatWidgetClass;

    UFUNCTION(Client, Unreliable)
        void CreateChatWidget();
    UFUNCTION(BlueprintCallable)
        void SetWidget();  
    UFUNCTION(BlueprintCallable)
        void BringUpChat();
    UFUNCTION(Server, Unreliable)
        void SendChatMessageToServer(const FString& message);
    UFUNCTION(Client, Unreliable)
        void SendMessageToClient(const FString& message);

protected:
    UPROPERTY()
        UChatWidget* chatWidget;
    UPROPERTY()
        bool isNameSetup = false;
};

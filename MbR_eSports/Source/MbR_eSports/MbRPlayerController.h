// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatSystem/ChatWidget.h"
#include "Net/UnrealNetwork.h"
#include "ChatSystem/ChatMessageWidget.h"
#include "GameFramework/PlayerController.h"
#include "MbRGameInstance.h"
#include "MbRPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateSentMessage, const FString&, chatString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateToxicSentMessage, const FString&, toxicChatString);

/*
Player Controller class to recieve input, create chat widget and own it for the purpose of runnning RPCs
Player Controller is also responsible for Ending Session
*/
UCLASS()
class MBR_ESPORTS_API AMbRPlayerController : public APlayerController
{
    GENERATED_BODY()
        virtual void SetupInputComponent() override;
        virtual void OnNetCleanup(UNetConnection* connection) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<UUserWidget> chatWidgetClass;

    UFUNCTION(BlueprintCallable)
        void Begin();
    UFUNCTION(Client, Unreliable)
        void CreateChatWidget();
    UFUNCTION(BlueprintCallable)
        void SetWidget();
    UFUNCTION(BlueprintCallable)
        void BringUpChat();
    UFUNCTION(BlueprintCallable)
        void HandleEndSession(bool successful);
    UFUNCTION(Server, Unreliable)
        void SendChatMessageToServer(const FString& message);
    UFUNCTION(Client, Unreliable)
        void SendMessageToClient(const FString& message);
    UFUNCTION()
        UMbRGameInstance* GetGameInstance() { return mbRGameInstance; }

protected:
    UPROPERTY()
        UChatWidget* chatWidget;
    UPROPERTY()
        bool isNameSetup = false;
    UPROPERTY()
        UMbRGameInstance* mbRGameInstance;

    UFUNCTION(NetMulticast, Reliable)
        void ClientOnEndSession();
};

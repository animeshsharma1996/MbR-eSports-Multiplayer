// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatSystem/ChatWidget.h"
#include "Net/UnrealNetwork.h"
#include "ChatSystem/ChatMessageWidget.h"
#include "GameFramework/PlayerState.h"
#include "MbRPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MBR_ESPORTS_API AMbRPlayerState : public APlayerState
{
    GENERATED_BODY()
        AMbRPlayerState(const class FObjectInitializer& PCIP);
        virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
        virtual void ClientInitialize(class AController* InController) override;
        virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<UUserWidget> chatWidgetClass;

    UFUNCTION(BlueprintCallable)
        void SetChatWidgetClass(TSubclassOf<UUserWidget> widget) { chatWidgetClass = widget; }
    UFUNCTION(BlueprintCallable)
        void SetWidget();    
    UFUNCTION(Server, Unreliable)
        void SetWidgetServer(UChatWidget* widget);

    UFUNCTION()
        void CreateChatWidget();
    UFUNCTION(Server, Unreliable)
        void SendChatMessageToServer(const FString& message);
    UFUNCTION(NetMulticast, Unreliable)
        void SendMessageToAll(const FString& message);

private:
    
    UPROPERTY(replicated)
        UChatWidget* chatWidget; 
    UPROPERTY(replicated)
        FString chatMessage;

 
};

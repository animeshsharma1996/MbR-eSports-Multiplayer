// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatSystem/ChatWidget.h"
#include "Net/UnrealNetwork.h"
#include "ChatSystem/ChatMessageWidget.h"
#include "GameFramework/PlayerController.h"
#include "MbRPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MBR_ESPORTS_API AMbRPlayerController : public APlayerController
{
	GENERATED_BODY()
        AMbRPlayerController(const class FObjectInitializer& PCIP);
        virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
        virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WidgetAssignment")
        TSubclassOf<UUserWidget> chatWidgetClass;

    UFUNCTION(Client, Unreliable)
        void CreateChatWidget();
    UFUNCTION(BlueprintCallable)
        void SetWidget();
    UFUNCTION(Server, Unreliable)
        void SetWidgetServer(UChatWidget* widget);
    UFUNCTION(Server, Unreliable)
        void SendChatMessageToServer(const FString& message);
    UFUNCTION(NetMulticast, Unreliable)
        void SendMessageToAll(const FString& message);

protected:
    UPROPERTY(replicated)
        UChatWidget* chatWidget;
    UPROPERTY(replicated)
        FString chatMessage;
};

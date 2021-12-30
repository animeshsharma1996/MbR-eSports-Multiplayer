#include "ServerSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UServerSlotWidget::OnServerInfoUpdate(FServerInfo serverInfo, UMbRGameInstance* gameInstance)
{
	joinButton->OnClicked.AddDynamic(this, &UServerSlotWidget::OnJoinButtonClicked);

	serverData = serverInfo;
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	FString currentMaxPlayers = FString::FromInt(serverData.currentPlayers) + " / " + FString::FromInt(serverData.maxPlayers);
	playersNumText->SetText(FText::AsCultureInvariant(currentMaxPlayers));
	arrayIndex = serverInfo.serverArrayIndex;
	mbRGameInstance = gameInstance;
}

void UServerSlotWidget::OnJoinButtonClicked()
{
	mbRGameInstance->JoinServer(arrayIndex, FName(serverData.serverName));
}
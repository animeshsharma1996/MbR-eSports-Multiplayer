#include "ServerSlotWidget.h"
#include "Components/TextBlock.h"

void UServerSlotWidget::OnServerInfoUpdate(FServerInfo serverInfo)
{
	serverData = serverInfo;
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	FString currentMaxPlayers = FString::FromInt(serverData.currentPlayers) + " / " + FString::FromInt(serverData.maxPlayers);
	playersNumText->SetText(FText::AsCultureInvariant(currentMaxPlayers));
}
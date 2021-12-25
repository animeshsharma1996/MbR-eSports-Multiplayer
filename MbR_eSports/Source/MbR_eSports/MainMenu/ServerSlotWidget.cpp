#include "ServerSlotWidget.h"
#include "Components/TextBlock.h"

bool UServerSlotWidget::Initialize()
{
	Super::Initialize();
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	playersNumText->SetText(FText::AsCultureInvariant(serverData.currentPlayers + " / " + serverData.maxPlayers));
	return true;
}

void UServerSlotWidget::OnServerInfoUpdate(FServerInfo serverInfo)
{
	serverData = serverInfo;
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	playersNumText->SetText(FText::AsCultureInvariant(serverData.currentPlayers + " / " + serverData.maxPlayers));
}
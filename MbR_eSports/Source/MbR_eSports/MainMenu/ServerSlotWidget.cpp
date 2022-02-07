#include "ServerSlotWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

/*
Function defined to be called as soon as the server slot widget is created and the game instance and server info are 
are passed as parameters. Also binds join button with the OnJoinButtonClicked function dynamically
*/
void UServerSlotWidget::OnServerInfoUpdate(FServerInfo serverInfo, UMbRGameInstance* gameInstance)
{
	joinButton->OnClicked.AddDynamic(this, &UServerSlotWidget::OnJoinButtonClicked);

	serverData = serverInfo;
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	FString currentMaxPlayers = FString::FromInt(serverData.currentPlayers) + " / " + FString::FromInt(serverData.maxPlayers);
	playersNumText->SetText(FText::AsCultureInvariant(currentMaxPlayers));
	lanText->SetText(((serverData.isLan) == true) ? FText::AsCultureInvariant("Yes") : FText::AsCultureInvariant("No"));
	pingText->SetText(FText::AsCultureInvariant(FString::FromInt(serverData.ping)));
	arrayIndex = serverInfo.serverArrayIndex;
	mbRGameInstance = gameInstance;
}

void UServerSlotWidget::OnJoinButtonClicked()
{
	mbRGameInstance->JoinServer(arrayIndex, FName(serverData.serverName));
}
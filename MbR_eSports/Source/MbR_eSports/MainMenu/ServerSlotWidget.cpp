#include "ServerSlotWidget.h"
#include "Components/TextBlock.h"

bool UServerSlotWidget::Initialize()
{
	Super::Initialize();
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	return true;
}

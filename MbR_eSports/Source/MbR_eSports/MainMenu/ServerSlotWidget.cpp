#include "Components/TextBlock.h"
#include "ServerSlotWidget.h"

bool UServerSlotWidget::Initialize()
{
	Super::Initialize();
	serverNameText->SetText(FText::AsCultureInvariant(serverData.serverName));
	return true;
}

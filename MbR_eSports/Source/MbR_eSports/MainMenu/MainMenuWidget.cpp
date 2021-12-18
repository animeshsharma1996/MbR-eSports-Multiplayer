// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

bool UMainMenuWidget::Initialize()
{
	Super::Initialize();

	hostButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HostButtonClicked);
	connectButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ConnectButtonClicked);
	exitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ExitButtonClicked);
	mbRGameInstance = Cast<UMbRGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	return true;
}

void UMainMenuWidget::HostButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->CreateServer();

		this->RemoveFromViewport();
	}
}

void UMainMenuWidget::ConnectButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		mbRGameInstance->JoinServer();

		this->RemoveFromViewport();
	}
}

void UMainMenuWidget::ExitButtonClicked()
{
	if (mbRGameInstance != NULL)
	{
		//mbRGameInstance->DestroySession();
	}
	APlayerController* SpecificPlayer = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), SpecificPlayer, EQuitPreference::Quit, true);
}


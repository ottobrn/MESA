// Fill out your copyright notice in the Description page of Project Settings.

#include "MesaUIManager.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/MesaDebugMenu.h"

UMesaUIManager* UMesaUIManager::Get(UWorld* World)
{
	if (World)
	{
		return UGameInstance::GetSubsystem<UMesaUIManager>(World->GetGameInstance());
	}
	return nullptr;
}

void UMesaUIManager::ToggleDebugMenu()
{
#if ALLOW_GAMEPLAY_DEBUGGER
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->IsLocalController())
	{
		if (!DebugMenuWidget)
		{
			UUserWidget* CreatedWidget = CreateWidget(PC, UMesaDebugMenu::StaticClass());
			DebugMenuWidget = Cast<UMesaDebugMenu>(CreatedWidget);
		}
		DebugMenuWidget->ToggleDebugMenu();
	}
#endif
}

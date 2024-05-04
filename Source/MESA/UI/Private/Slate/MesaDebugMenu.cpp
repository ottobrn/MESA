// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaDebugMenu.h"
#include "ImGuiModule.h"

void UMesaDebugMenu::ToggleDebugMenu()
{
	if (FImGuiModule::IsAvailable())
	{
		if (!bIsRegistered)
		{
			ImGuiDelegate = FImGuiDelegate::CreateUObject(this, &UMesaDebugMenu::OnDraw);
			ImGuiHandle = FImGuiModule::Get().AddWorldImGuiDelegate(ImGuiDelegate);
			bIsRegistered = true;
		}
		bIsDebugEnabled = !bIsDebugEnabled;
	}
}

void UMesaDebugMenu::OnDraw()
{
	if (bIsDebugEnabled)
	{
		if (ImGui::Begin("Debug Menu"))
		{
			ImGui::EndMenu();	
		}
	}
}

void UMesaDebugMenu::NativeDestruct()
{
	Super::NativeDestruct();
	FImGuiModule::Get().RemoveImGuiDelegate(ImGuiHandle);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaDebugMenu.h"
#include "MESA/MESA.h"
#include <imgui_internal.h>
#include "ImGuiModule.h"
#include "MESA/Debug/MesaDebugManager.h"

void UMesaDebugMenu::ToggleDebugMenu()
{
	if (FImGuiModule::IsAvailable())
	{
		if (!bIsRegistered)
		{
			ImGuiDelegate = FImGuiDelegate::CreateUObject(this, &UMesaDebugMenu::OnDraw);
			ImGuiHandle = FImGuiModule::Get().AddWorldImGuiDelegate(ImGuiDelegate);
			MesaDebugManager = UMesaDebugManager::Get(GetWorld());
			
			SetupWindowStyle();
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
			DrawAnimationSection();
			ImGui::EndMenu();	
		}
	}
}

void UMesaDebugMenu::NativeDestruct()
{
	Super::NativeDestruct();
	FImGuiModule::Get().RemoveImGuiDelegate(ImGuiHandle);
}

void UMesaDebugMenu::SetupWindowStyle()
{
	ImGuiContext* CurrentContext = ImGui::GetCurrentContext();
	CurrentContext->Style.FrameRounding = 2.f;
	CurrentContext->Style.WindowPadding = ImVec2(10.f, 5.f);
	CurrentContext->Style.ItemSpacing = ImVec2(8.f, 6.f);
	CurrentContext->Style.WindowRounding = 6.f;
}

void UMesaDebugMenu::DrawAnimationSection()
{
	if (ImGui::CollapsingHeader("Animations"))
	{
		ImGui::Checkbox("Show movement stats", &bIsMovementStatsOpened);
		if (bIsMovementStatsOpened)
		{
			if (ImGui::Begin("Movement Stat"), &bIsMovementStatsOpened)
			{
				ImGui::Text("Character Gait: %s", TCHAR_TO_UTF8(*EnumToString(MesaDebugManager->GetCharacterGait())));
				ImGui::Text("Character Stance: %s", TCHAR_TO_UTF8(*EnumToString(MesaDebugManager->GetCharacterStance())));
				ImGui::Text("Movement State: %s", TCHAR_TO_UTF8(*EnumToString(MesaDebugManager->GetCharacterMovementState())));
				ImGui::Text("Movement Direction: %s", TCHAR_TO_UTF8(*EnumToString(MesaDebugManager->GetCharacterMovementDirection())));
				ImGui::End();
			}
		}
	}
}

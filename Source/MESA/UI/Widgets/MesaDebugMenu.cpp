// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaDebugMenu.h"
#include "MESA/MESA.h"
#include <imgui_internal.h>
#include "ImGuiModule.h"
#include "MESA/Debug/MesaDebugHelpers.h"
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
	const TArray<FGameDebugTextInfo>* AnimInfos = MesaDebugManager->GetDebugInfo().Find(AnimHeaderName);
	const FString* Key = MesaDebugManager->GetDebugInfo().FindKey(*AnimInfos);
	if (!Key || !AnimInfos)
	{
		return;
	}
	
	const char* Label = ToCharPtr(*Key);
	if (ImGui::CollapsingHeader(Label))
	{
		if (Label == AnimHeaderName)
		{
			ImGui::Checkbox("Show movement stats", &bIsMovementStatsOpened);
			ImGui::Checkbox("Show movement values", &bIsMovementValuesOpened);
			for (const FGameDebugTextInfo& TextInfo : *AnimInfos)
			{
				if (TextInfo.MenuName == MovementStatsMenuName)
				{
					if (bIsMovementStatsOpened)
					{
						if (ImGui::Begin(ToCharPtr(MovementStatsMenuName)), &bIsMovementStatsOpened)
						{
							ImGui::Text(ToCharPtr(TextInfo.Header + ": %s"), ToCharPtr(TextInfo.Value));
							ImGui::Separator();
							ImGui::End();
						}
					}
				}
				else if (TextInfo.MenuName == MovementValuesMenuName)
				{
					if (bIsMovementValuesOpened)
					{
						if (ImGui::Begin(ToCharPtr(MovementStatsMenuName)), &bIsMovementValuesOpened)
						{
							ImGui::Text(ToCharPtr(TextInfo.Header + ": %s"), ToCharPtr(TextInfo.Value));
							ImGui::Separator();
							ImGui::End();
						}
					}
				}
			}
		}
	}
}

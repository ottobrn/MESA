// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaDebugMenu.h"
#include "MESA/MESA.h"
#include <imgui_internal.h>
#include "ImGuiModule.h"
#include "Kismet/GameplayStatics.h"
#include "MESA/Character/MesaCharacterBase.h"
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
			MesaCharacter = Cast<AMesaCharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			
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
	ImVec4* Colors = ImGui::GetStyle().Colors;
	Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
	Colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	Colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	Colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	Colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	Colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	Colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	Colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	Colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
	Colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	Colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	Colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	Colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	Colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	Colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	Colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	Colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	Colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
	Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
	Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

	ImGuiStyle& Style = ImGui::GetStyle();
	Style.WindowPadding = ImVec2(8.00f, 8.00f);
	Style.FramePadding = ImVec2(5.00f, 2.00f);
	Style.CellPadding = ImVec2(6.00f, 6.00f);
	Style.ItemSpacing = ImVec2(6.00f, 6.00f);
	Style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
	Style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
	Style.IndentSpacing = 25;
	Style.ScrollbarSize = 15;
	Style.GrabMinSize = 10;
	Style.WindowBorderSize = 1;
	Style.ChildBorderSize = 1;
	Style.PopupBorderSize = 1;
	Style.FrameBorderSize = 1;
	Style.TabBorderSize = 1;
	Style.WindowRounding = 7;
	Style.ChildRounding = 4;
	Style.FrameRounding = 3;
	Style.PopupRounding = 4;
	Style.ScrollbarRounding = 9;
	Style.GrabRounding = 3;
	Style.LogSliderDeadzone = 4;
	Style.TabRounding = 4;
}

void UMesaDebugMenu::DrawAnimationSection()
{
	const TArray<FGameDebugTextInfo>* AnimInfos = MesaDebugManager->GetDebugInfo().Find(AnimHeaderName);
	const FName* Key = MesaDebugManager->GetDebugInfo().FindKey(*AnimInfos);
	if (!Key || !AnimInfos)
	{
		return;
	}
	
	const char* Label = ToCharPtr(*Key->ToString());
	if (ImGui::CollapsingHeader(Label))
	{
		if (Label == AnimHeaderName)
		{
			ImGui::Checkbox("Show movement stats", &bIsMovementStatsOpened);
			ImGui::Checkbox("Show movement values", &bIsMovementValuesOpened);
			for (const FGameDebugTextInfo& TextInfo : *AnimInfos)
			{
				if (TextInfo.MenuName.IsNone() && TextInfo.DynamicValue.IsValid())
				{
					switch (TextInfo.DebugCategory)
					{
					case EDebugCategory::Checkbox:
						ImGui::Checkbox(ToCharPtr(TextInfo.Header.ToString()), reinterpret_cast<bool*>(TextInfo.DynamicValue.Get()));
						continue;
					case EDebugCategory::Slider:
						continue;
					default:
						continue;
					}
				}
				if (TextInfo.MenuName == MovementStatsMenuName)
				{
					if (bIsMovementStatsOpened)
					{
						if (ImGui::Begin(ToCharPtr(MovementStatsMenuName.ToString())), &bIsMovementStatsOpened)
						{
							ImGui::Text(ToCharPtr(TextInfo.Header.ToString() + ": %s"), ToCharPtr(TextInfo.Value));
							ImGui::Separator();
							ImGui::End();
						}
					}
				}
				else if (TextInfo.MenuName == MovementValuesMenuName)
				{
					if (bIsMovementValuesOpened)
					{
						if (ImGui::Begin(ToCharPtr(MovementStatsMenuName.ToString())), &bIsMovementValuesOpened)
						{
							ImGui::Text(ToCharPtr(TextInfo.Header.ToString() + ": %s"), ToCharPtr(TextInfo.Value));
							ImGui::Separator();
							ImGui::End();
						}
					}
				}
			}
		}
	}
}

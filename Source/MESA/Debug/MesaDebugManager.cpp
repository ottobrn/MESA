// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaDebugManager.h"

void UMesaDebugManager::Deinitialize()
{
	Super::Deinitialize();
	DebugInfo.Empty();
}

UMesaDebugManager* UMesaDebugManager::Get(UWorld* World)
{
#if ALLOW_GAMEPLAY_DEBUGGER
	if (World)
	{
		return UGameInstance::GetSubsystem<UMesaDebugManager>(World->GetGameInstance());
	}
#endif
	return nullptr;
}


void UMesaDebugManager::EmplaceDebugInfo(const FName& InHeader, const FGameDebugTextInfo& InInfo)
{
	if (!DebugInfo.Contains(InHeader))
	{
		TArray<FGameDebugTextInfo> InitArray = { InInfo };
		DebugInfo.Emplace(InHeader, InitArray);
		return;
	}

	TArray<FGameDebugTextInfo>* FoundInfo = DebugInfo.Find(InHeader);
	if (FoundInfo)
	{
		auto FindPredicate = [&InInfo](const FGameDebugTextInfo& B)
		{
			return InInfo.Header == B.Header;
		};
		
		FGameDebugTextInfo* FoundStruct = FoundInfo->FindByPredicate(FindPredicate);
		if (FoundStruct)
		{
			FoundStruct->Value = InInfo.Value;
		}
		else
		{
			FoundInfo->Add(InInfo);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MesaDebugManager.h"

#include "MESA/Animation/MesaCharacterAnimInstance.h"
#include "MESA/Character/MesaCharacterBase.h"

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

void UMesaDebugManager::SetCharacter(AMesaCharacterBase* NewCharacter)
{
	CharacterInstance = NewCharacter;
}

ECharacterGait UMesaDebugManager::GetCharacterGait() const
{
	if (CharacterInstance)
	{
		return CharacterInstance->GetCharacterGait();
	}
	return ECharacterGait::None;
}

ECharacterStance UMesaDebugManager::GetCharacterStance() const
{
	if (CharacterInstance)
	{
		return CharacterInstance->GetCharacterStance();
	}
	return ECharacterStance::None;
}

ECharacterMovementDirection UMesaDebugManager::GetCharacterMovementDirection() const
{
	if (CharacterInstance)
	{
		if (UMesaCharacterAnimInstance* AnimInstance = Cast<UMesaCharacterAnimInstance>(CharacterInstance->GetMesh()->GetAnimInstance()))
		{
			return AnimInstance->GetMovementDirection().Direction;
		}
	}
	return ECharacterMovementDirection::None;
}

ECharacterMovementState UMesaDebugManager::GetCharacterMovementState() const
{
	if (CharacterInstance)
	{
		return CharacterInstance->GetMovementState();
	}
	return ECharacterMovementState::None;
}

void UMesaDebugManager::EmplaceDebugInfo(const FString& InHeader, const FGameDebugTextInfo& InInfo)
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

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MESA : ModuleRules
{
	public MESA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "Slate", "SlateCore"});

		PrivateDependencyModuleNames.AddRange(new string[] { "AnimGraphRuntime", "GameplayDebugger" });

		if (Target.Configuration == UnrealTargetConfiguration.Shipping)
		{
			PublicDefinitions.AddRange(new string[] { "ALLOW_GAMEPLAY_DEBUGGER=0"});
		}
		else
		{
			PublicDefinitions.AddRange(new string[] { "ALLOW_GAMEPLAY_DEBUGGER=1"});
			PrivateDependencyModuleNames.Add("ImGui");
		}
		

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

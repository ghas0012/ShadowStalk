// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShadowStalk : ModuleRules
{
	public ShadowStalk(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDefinitions.Add("STEAM_ENABLED");

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemNull", "Steamworks", "UMG" /*,"BUITween"*/});
		DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		MinFilesUsingPrecompiledHeaderOverride = 1;
		bFasterWithoutUnity = true;

		//PublicIncludePaths.AddRange(new string[] { "../Plugins/ue4-uitween-master/Source/UITween/Public", "../Plugins/ue4-uitween-master/Source/UITween/Private" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

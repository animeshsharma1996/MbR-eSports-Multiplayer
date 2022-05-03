// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MbR_eSports : ModuleRules
{
	public MbR_eSports(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		//Code block for faster compile time for UE4 C++
		PrivatePCHHeaderFile = "MbR_eSports.h";
		MinFilesUsingPrecompiledHeaderOverride = 1;
		bUseUnity = false;

		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"SlateCore",
			"OnlineSubsystem",       
			"OnlineSubsystemUtils",
			"OnlineSubsystemSteam", //Build depends on OnlineSubsystemSteam Module
			"Steamworks"
		});
	}
}

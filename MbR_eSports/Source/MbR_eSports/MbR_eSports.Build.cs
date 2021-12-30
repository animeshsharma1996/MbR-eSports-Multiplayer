// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MbR_eSports : ModuleRules
{
	public MbR_eSports(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"OnlineSubsystem",
			"OnlineSubsystemSteam",
			"Networking", 
			"Sockets"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}

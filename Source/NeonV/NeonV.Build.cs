// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NeonV : ModuleRules
{
	public NeonV(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}

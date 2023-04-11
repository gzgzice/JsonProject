// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JsonProject : ModuleRules
{
	public JsonProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Json" });
	}
}

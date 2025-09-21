// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Minesweeper : ModuleRules
{
	public Minesweeper(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
            {
				"Core", "CoreUObject", "Engine", "Slate", "SlateCore"
            }
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
            {
            "ApplicationCore",
            "InputCore",
            "EditorFramework",
            "UnrealEd",
            "ToolMenus",     // toolbar/menu
            "LevelEditor",   // Level Editor toolbar extension
            "Projects",
            "EditorStyle"
            }
		);
	}
}

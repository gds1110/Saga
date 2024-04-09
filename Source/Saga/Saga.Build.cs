// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Saga : ModuleRules
{
	public Saga(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "Saga" });


        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine", 
			"InputCore",
			"HeadMountedDisplay", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara", 
			"EnhancedInput",
			"ProceduralMeshComponent",
			"UMG",
			"GameplayTasks"
		});

        PrivateDependencyModuleNames.AddRange(new string[] { 
			"Slate",
			"SlateCore" 
		});


    }
}

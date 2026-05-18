// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProyectoFinal : ModuleRules
{
	public ProyectoFinal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ProyectoFinal",
			"ProyectoFinal/Variant_Platforming",
			"ProyectoFinal/Variant_Platforming/Animation",
			"ProyectoFinal/Variant_Combat",
			"ProyectoFinal/Variant_Combat/AI",
			"ProyectoFinal/Variant_Combat/Animation",
			"ProyectoFinal/Variant_Combat/Gameplay",
			"ProyectoFinal/Variant_Combat/Interfaces",
			"ProyectoFinal/Variant_Combat/UI",
			"ProyectoFinal/Variant_SideScrolling",
			"ProyectoFinal/Variant_SideScrolling/AI",
			"ProyectoFinal/Variant_SideScrolling/Gameplay",
			"ProyectoFinal/Variant_SideScrolling/Interfaces",
			"ProyectoFinal/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

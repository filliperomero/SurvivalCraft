// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SurvivalCraft : ModuleRules
{
	public SurvivalCraft(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "GeometryCollectionEngine", "FieldSystemEngine", "PhysicsCore", "Niagara", "AdvancedSessions", "AdvancedSteamSessions" });
		
		// PrivateDependencyModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions"});
		// PrivateIncludePathModuleNames.AddRange(new string[] { "AdvancedSessions", "AdvancedSteamSessions"});
	}
}

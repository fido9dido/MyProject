// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class GlobalsModule : ModuleRules
{
    public GlobalsModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "MassSpawner"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core"
            ,"CoreUObject"
            ,"Engine"
            ,"NavigationSystem"
            ,"CommonInput"

            ,"MassEntity"
            ,"StructUtils"
            ,"DeveloperSettings"


        });

        // PublicIncludePaths.AddRange(new string[] { ModuleDirectory + "/Public/Components/" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
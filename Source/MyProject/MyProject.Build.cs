// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core"
            ,"CoreUObject"
            ,"Engine"
            ,"InputCore"
            ,"EnhancedInput"
            ,"StructUtils"

            ,"GeometryScriptingCore"
            ,"GeometryScriptingEditor"

            ,"MassRepresentation"
            ,"MassEntity"
            ,"MassSimulation"
            ,"MassCommon"
            ,"MassNavigation"
            ,"MassCrowd"
            ,"MassSignals"
            ,"MassAIBehavior"
            ,"MassActors"
            ,"MassMovement"
            ,"MassLOD"

            ,"ControlRig"
            ,"RigVM"
            ,"MotionWarping"
            ,"DeveloperSettings"
            ,"ContextualAnimation"
            ,"AnimGraphRuntime"

            ,"StateTreeModule"
            ,"StateTreeEditorModule"


            ,"NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] { ModuleDirectory + "/Public/Components/" });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}

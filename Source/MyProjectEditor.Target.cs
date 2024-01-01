// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProjectEditorTarget : TargetRules
{
    public MyProjectEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.Add("MyProject");
        ExtraModuleNames.Add("MassModule");
        ExtraModuleNames.Add("GlobalsModule");
    }
}

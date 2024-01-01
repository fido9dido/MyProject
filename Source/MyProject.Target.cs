// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class MyProjectTarget : TargetRules
{
    public MyProjectTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
        ExtraModuleNames.Add("MyProject");
        ExtraModuleNames.Add("MassModule");
        ExtraModuleNames.Add("GlobalsModule");
        bUseInlining = false;
    }
}

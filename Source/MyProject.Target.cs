// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MyProjectTarget : TargetRules
{
    public MyProjectTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
        ExtraModuleNames.Add("MyProject");
        ExtraModuleNames.Add("MassModule");
        ExtraModuleNames.Add("GlobalsModule");
        bUseInlining = false;

    }
}

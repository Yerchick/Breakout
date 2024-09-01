// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BPOEditorTarget : TargetRules
{
	public BPOEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "BPO" } );
	}
}

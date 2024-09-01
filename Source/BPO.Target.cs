// Breakout Portal One Game. Copyright Yerchick Dmytro. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BPOTarget : TargetRules
{
	public BPOTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "BPO" } );
	}
}

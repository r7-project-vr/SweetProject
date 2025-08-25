using UnrealBuildTool;

public class SweetProject : ModuleRules
{
    public SweetProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Projects", "AIModule", "GameplayTasks" });

        PrivateDependencyModuleNames.AddRange(new string[] { });
    }
}
using UnrealBuildTool;

public class SweetProject : ModuleRules
{
    public SweetProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Projects",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "KATVRUniversalSDK",
            "Niagara",
            "UMG",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "ASerialCom" });

        if (Target.bBuildEditor == true)
        {
            // エディタ専用のモジュールをここに追加する
            PublicDependencyModuleNames.Add("UnrealEd");
            PublicDependencyModuleNames.Add("Cascade");
            PublicDependencyModuleNames.Add("GeometryCollectionEngine");
            PublicDependencyModuleNames.Add("Slate");
            PublicDependencyModuleNames.Add("SlateCore");
            // その他、エディタ専用モジュール
        }
    }
}
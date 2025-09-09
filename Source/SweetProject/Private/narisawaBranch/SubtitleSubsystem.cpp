#include "narisawaBranch/SubtitleSubsystem.h"
#include "narisawaBranch/SubtitleActor.h"

void USubtitleSubsystem::RegisterSubtitleActor(ASubtitleActor* SubtitleActor)
{
    if (!SubtitleActor || SubtitleActor->SubtitleID.IsNone())
    {
        // アクターが無効か、IDが設定されていない場合は何もしない
        return;
    }
    SubtitleActorMap.Add(SubtitleActor->SubtitleID, SubtitleActor);
}

void USubtitleSubsystem::ShowSubtitleByID(const FName& SubtitleID)
{
    if (ASubtitleActor* FoundActor = SubtitleActorMap.FindRef(SubtitleID))
    {
        FoundActor->ShowSubtitle();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SubtitleSubsystem: SubtitleActor with ID '%s' not found."), *SubtitleID.ToString());
    }
}

void USubtitleSubsystem::HideSubtitleByID(const FName& SubtitleID)
{
    if (ASubtitleActor* FoundActor = SubtitleActorMap.FindRef(SubtitleID))
    {
        FoundActor->HideSubtitle();
    }
}

void USubtitleSubsystem::Deinitialize()
{
    // マップをクリアして、古いレベルのアクターへの参照を解放する
    SubtitleActorMap.Empty();
    Super::Deinitialize();
}
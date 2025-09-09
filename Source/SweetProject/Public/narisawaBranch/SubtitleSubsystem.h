#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SubtitleSubsystem.generated.h"

class ASubtitleActor;

UCLASS()
class SWEETPROJECT_API USubtitleSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // ASubtitleActorがBeginPlayで自身を登録するために呼び出す
    void RegisterSubtitleActor(ASubtitleActor* SubtitleActor);

    // 外部からIDを指定して字幕を表示する
    void ShowSubtitleByID(const FName& SubtitleID);

    // 外部からIDを指定して字幕を非表示にする
    void HideSubtitleByID(const FName& SubtitleID);

    // レベルが切り替わった際などに登録をクリアする
    virtual void Deinitialize() override;

private:
    // SubtitleIDとSubtitleActorのポインタをマップで管理する
    TMap<FName, TObjectPtr<ASubtitleActor>> SubtitleActorMap;
};
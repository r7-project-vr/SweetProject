#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubtitleActor.generated.h"

class UWidgetComponent;

UCLASS()
class SWEETPROJECT_API ASubtitleActor : public AActor
{
    GENERATED_BODY()

public:
    ASubtitleActor();

    // 字幕を識別するためのID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Subtitle")
    FName SubtitleID;

    // 字幕を表示する関数
    void ShowSubtitle();

    // 字幕を非表示にする関数
    void HideSubtitle();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> DefaultSceneRoot;

    // 3D空間にウィジェットを表示するためのコンポーネント
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UWidgetComponent> SubtitleWidgetComponent;
};
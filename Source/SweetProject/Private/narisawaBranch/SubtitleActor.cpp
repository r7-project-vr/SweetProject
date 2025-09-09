#include "narisawaBranch/SubtitleActor.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "narisawaBranch/SubtitleSubsystem.h"

ASubtitleActor::ASubtitleActor()
{
    PrimaryActorTick.bCanEverTick = false;

    DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
    RootComponent = DefaultSceneRoot;

    SubtitleWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("SubtitleWidgetComponent"));
    SubtitleWidgetComponent->SetupAttachment(RootComponent);
    SubtitleWidgetComponent->SetVisibility(false); // 初期状態では非表示
}

void ASubtitleActor::BeginPlay()
{
    Super::BeginPlay();

    // ゲームインスタンスからSubtitleSubsystemを取得して、自身を登録する
    if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
    {
        if (USubtitleSubsystem* SubtitleSubsystem = GameInstance->GetSubsystem<USubtitleSubsystem>())
        {
            SubtitleSubsystem->RegisterSubtitleActor(this);
        }
    }
}

void ASubtitleActor::ShowSubtitle()
{
    SubtitleWidgetComponent->SetVisibility(true);
}

void ASubtitleActor::HideSubtitle()
{
    SubtitleWidgetComponent->SetVisibility(false);
}
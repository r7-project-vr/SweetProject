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
    HideSubtitle();
}

void ASubtitleActor::ShowSubtitle()
{
    if (SubtitleWidgetComponent)
    {
        SubtitleWidgetComponent->SetVisibility(true);
    }
}

void ASubtitleActor::HideSubtitle()
{
    if (SubtitleWidgetComponent)
    {
        SubtitleWidgetComponent->SetVisibility(false);
    }
}
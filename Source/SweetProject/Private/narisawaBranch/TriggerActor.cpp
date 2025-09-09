#include "narisawaBranch/TriggerActor.h"
#include "Components/BoxComponent.h"
#include "narisawaBranch/SubtitleSubsystem.h" 
#include "Kismet/GameplayStatics.h"

ATriggerActor::ATriggerActor()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
    RootComponent = TriggerVolume;

    TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::OnOverlapBegin);
}

void ATriggerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ShowSubtitle();
}

// 字幕表示処理の実装
void ATriggerActor::ShowSubtitle()
{
    // SubtitleSubsystemを取得
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (USubtitleSubsystem* SubtitleSubsystem = GameInstance->GetSubsystem<USubtitleSubsystem>())
        {
            // IDを指定して字幕を表示するようリクエスト
            SubtitleSubsystem->ShowSubtitleByID(SubtitleID);
        }
    }
}
#include "narisawaBranch/TriggerActor.h"
#include "Components/BoxComponent.h"
#include "narisawaBranch/SubtitleSubsystem.h" 
//2025 9.10 得丸陽生
#include "TokumaruBranch/Actor/CPP_GetSpace.h"
//2025 9.10 得丸陽生 end
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


    //2025 09.10 得丸陽生 
    SearchPickupActor();
    //2025 09.10 得丸陽生 end
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

//2025 09.10 得丸陽生
void ATriggerActor::SearchPickupActor()
{
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACPP_GetSpace::StaticClass(), foundActors);


    if (foundActors.Num() > 0)
    {
        AActor* swordPickupActor = foundActors[0]; // 条件に応じて選択可

        if (ACPP_GetSpace* getSpace = Cast<ACPP_GetSpace>(swordPickupActor)) {
            getSpace->FlashActor();
        }
    }
}
//2025 09.10 得丸陽生 end

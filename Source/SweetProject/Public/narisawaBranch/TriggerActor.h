#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerActor.generated.h"

class UBoxComponent;

UCLASS()
class SWEETPROJECT_API ATriggerActor : public AActor
{
    GENERATED_BODY()

public:
    ATriggerActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* TriggerVolume;

    // オーバーラップイベント用の関数
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subtitle")
    FName SubtitleID;

public:
    UFUNCTION(BlueprintCallable, Category = "Subtitle")
    void ShowSubtitle();
};
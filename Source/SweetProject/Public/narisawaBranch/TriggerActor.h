#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerActor.generated.h"

class UBoxComponent;

//2025 9.10 得丸陽生
class ACPP_GetSpace;
//2025 9.10 得丸陽生 end

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


    //2025 9.10 得丸陽生
    UFUNCTION(BlueprintCallable, Category = "Subtitle")
    void SearchPickupActor();
    //2025 9.10 得丸陽生 end
};
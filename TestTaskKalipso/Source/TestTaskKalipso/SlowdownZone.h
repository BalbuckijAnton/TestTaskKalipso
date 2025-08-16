#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlowdownZone.generated.h"


UCLASS()
class TESTTASKKALIPSO_API ASlowdownZone : public AActor
{
    GENERATED_BODY()

public:
    ASlowdownZone();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Slowdown")
    float SlowdownMultiplier = 0.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
    class UBoxComponent* TriggerZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UStaticMeshComponent* ZoneMesh;

    TMap<AActor*, TMap<FName, FVector>> OriginalSpeeds;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, Category = "Visual")
    UMaterialInterface* ZoneMaterial;
};

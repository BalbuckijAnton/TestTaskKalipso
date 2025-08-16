// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FatigueZone.generated.h"

class UStaminaComponent;

UCLASS()
class TESTTASKKALIPSO_API AFatigueZone : public AActor
{
	GENERATED_BODY()

public:
	AFatigueZone();

protected:
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
    class UBoxComponent* TriggerZone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UStaticMeshComponent* ZoneMesh;

    TMap<AActor*, float> OldStamina;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(EditAnywhere, Category = "Visual")
    UMaterialInterface* ZoneMaterial;

	UPROPERTY(EditAnywhere, Category = "Fatigue")
	float FatigueMultiplier = 3.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SlowdownZone.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKKALIPSO_API ASlowdownZone : public ATriggerBox
{
	GENERATED_BODY()

public:
    ASlowdownZone();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Slowdown")
    float SlowdownMultiplier = 0.5f;

    UFUNCTION()
    void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

    TMap<AActor*, TMap<FName, FVector>> OriginalSpeeds;
};

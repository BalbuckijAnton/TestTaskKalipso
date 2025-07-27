// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "StartFinishTrigger.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKKALIPSO_API AStartFinishTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
    AStartFinishTrigger();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

    UFUNCTION()
    void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
};

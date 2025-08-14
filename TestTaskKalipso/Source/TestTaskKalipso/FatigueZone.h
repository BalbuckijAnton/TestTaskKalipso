// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "FatigueZone.generated.h"

class UStaminaComponent;

UCLASS()
class TESTTASKKALIPSO_API AFatigueZone : public ATriggerBox
{
	GENERATED_BODY()

public:
	AFatigueZone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	TMap<AActor*, float> OldStamina;

private:

	UPROPERTY(EditAnywhere, Category = "Fatigue")
	float FatigueMultiplier = 3.0f;
};

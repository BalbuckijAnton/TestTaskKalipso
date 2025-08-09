// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DamageZone.generated.h"

class UHealthComponent;

/**
 * 
 */
UCLASS()
class TESTTASKKALIPSO_API ADamageZone : public ATriggerBox
{
	GENERATED_BODY()

public:
	ADamageZone();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	TMap<AActor*, FTimerHandle> ActiveTimers;

	void ApplyDamageToActor(UHealthComponent* ActorHealthComponent);

private:

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamagePerSecond = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageIntervalSecond = 0.5f;
};

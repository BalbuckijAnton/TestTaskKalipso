// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageZone.generated.h"

class UHealthComponent;

UCLASS()
class TESTTASKKALIPSO_API ADamageZone : public AActor
{
	GENERATED_BODY()

public:
	ADamageZone();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	class UBoxComponent* TriggerZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class UStaticMeshComponent* ZoneMesh;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Visual")
	UMaterialInterface* ZoneMaterial;

	TMap<AActor*, FTimerHandle> ActiveTimers;

	void ApplyDamageToActor(UHealthComponent* ActorHealthComponent);

private:

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamagePerSecond = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageIntervalSecond = 0.5f;
};

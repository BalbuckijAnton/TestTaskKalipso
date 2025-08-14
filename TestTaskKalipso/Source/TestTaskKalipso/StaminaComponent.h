// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDepletedSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStaminaChangedSignature, UStaminaComponent*, StaminaComp, float, MaxStamina, float, CurStamina);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TESTTASKKALIPSO_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float DefaultStamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float Stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float IncreaseStaminaPerSecond = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float RegenerationStaminaPerSecond = 10.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float MinStaminaForSprint = 5.0f;

	bool IsSprint = false;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	float TimeToBroadcastSec = 0.1f;

	float BroadCastTimer = 0.0f;

public:
	float GetMaxStamina() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Delegate for broadcasting health changes
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStaminaChangedSignature OnStaminaChanged;
	
	// Stamina is completely gone
	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaDepletedSignature OnStaminaDepleted;

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	bool SprintTrigger(bool IsOn);

	float SetMaxStamina(float NewMaxStamina);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	Stamina = DefaultStamina;
	IsSprint = false;
	// ...
}

float UStaminaComponent::GetMaxStamina() const
{
	return DefaultStamina;
}

// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsSprint)
	{
		Stamina = FMath::Clamp(Stamina - IncreaseStaminaPerSecond * DeltaTime, 0.0f, DefaultStamina);
		OnStaminaChanged.Broadcast(this, DefaultStamina, Stamina);
		if (FMath::IsNearlyZero(Stamina))
		{
			IsSprint = false;
			OnStaminaDepleted.Broadcast();
		}
	}
	else
	{
		if (!FMath::IsNearlyEqual(Stamina, DefaultStamina)) 
		{
			Stamina = FMath::Clamp(Stamina + RegenerationStaminaPerSecond * DeltaTime, 0.0f, DefaultStamina);
			OnStaminaChanged.Broadcast(this, DefaultStamina, Stamina);
		}
	}
}

bool UStaminaComponent::SprintTrigger(bool IsOn)
{
	if (IsOn)
	{
		if (Stamina > MinStaminaForSprint)
		{
			IsSprint = true;
		}
		return IsSprint;
	}
	else
	{
		return (IsSprint = false);
	}
}


float UStaminaComponent::SetMaxStamina(float NewMaxStamina)
{
	Swap(NewMaxStamina, DefaultStamina);
	Stamina = FMath::Min(Stamina, DefaultStamina);
	OnStaminaChanged.Broadcast(this, DefaultStamina, Stamina);
	return NewMaxStamina;
}
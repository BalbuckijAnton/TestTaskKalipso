// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RaceGameHUD.generated.h"

class UHealthComponent;
class UStaminaComponent;
class UHealthUserWidget;
class UStaminaUserWidget;
class UUserWidget;

UCLASS()
class TESTTASKKALIPSO_API ARaceGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UHealthUserWidget> HealthWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UStaminaUserWidget> StaminaWidgetClass;

    UPROPERTY()
    UHealthUserWidget* HealthWidget;

    UPROPERTY()
    UStaminaUserWidget* StaminaWidget;

    UFUNCTION()
    void OnHealthChanged(UHealthComponent* HealthComp, float Health, float Damage);

    UFUNCTION()
    void OnStaminaChanged(UStaminaComponent* StaminaComp, float MaxStamina, float CurStamina);

    UHealthComponent* HealthComponent;
    UStaminaComponent* StaminaComponent;
public:
    ARaceGameHUD();

    virtual void DrawHUD() override;

    void UpdateHealth(float Health, float MaxHealth);
    void UpdateStamina(float Stamina, float MaxStamina);

private:
    static FString ConvertTimeFloatToString(float TimeSeconds);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RaceGameHUD.generated.h"

class UHealthComponent;
class UHealthUserWidget; // если у тебя кастомный C++ класс UI
class UUserWidget;

UCLASS()
class TESTTASKKALIPSO_API ARaceGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UHealthUserWidget> HealthWidgetClass;

    UPROPERTY()
    UHealthUserWidget* HealthWidget;

    UFUNCTION()
    void OnHealthChanged(UHealthComponent* HealthComp, float Health, float Damage);

    UHealthComponent* HealthComponent;
public:
    ARaceGameHUD();

    virtual void DrawHUD() override;

    void UpdateHealth(float Health, float MaxHealth);

private:
    static FString ConvertTimeFloatToString(float TimeSeconds);
};

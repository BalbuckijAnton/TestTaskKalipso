// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthUserWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class TESTTASKKALIPSO_API UHealthUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetHealth(float CurrentHealth, float MaxHealth);

    UProgressBar* GetHealthBar();
    UTextBlock* GetHealthText();

protected:
    virtual void NativeConstruct() override;

    // Привязываемые виджеты из Blueprint
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* HealthText;
};
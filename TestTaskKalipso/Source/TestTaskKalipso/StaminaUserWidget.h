// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StaminaUserWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class TESTTASKKALIPSO_API UStaminaUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetStamina(float CurrentStamina, float MaxStamina);

    UProgressBar* GetStaminaBar();
    UTextBlock* GetStaminaText();

protected:
    virtual void NativeConstruct() override;

    // Привязываемые виджеты из Blueprint
    UPROPERTY(meta = (BindWidget))
    UProgressBar* StaminaBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* StaminaText;
};

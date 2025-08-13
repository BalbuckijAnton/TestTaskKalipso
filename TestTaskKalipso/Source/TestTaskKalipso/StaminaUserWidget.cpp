// Fill out your copyright notice in the Description page of Project Settings.


#include "StaminaUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStaminaUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Можно инициализировать виджет тут при старте, если нужно
}

void UStaminaUserWidget::SetStamina(float CurrentStamina, float MaxStamina)
{
    if (StaminaBar)
    {
        StaminaBar->SetPercent(CurrentStamina / MaxStamina);
    }

    if (StaminaText)
    {
        FString StaminaStr = FString::Printf(TEXT("%d / %d"),
            FMath::RoundToInt(CurrentStamina), FMath::RoundToInt(MaxStamina));

        StaminaText->SetText(FText::FromString(StaminaStr));
    }
}

UProgressBar* UStaminaUserWidget::GetStaminaBar()
{
    return StaminaBar;
}

UTextBlock* UStaminaUserWidget::GetStaminaText()
{
    return StaminaText;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthUserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Можно инициализировать виджет тут при старте, если нужно
}

void UHealthUserWidget::SetHealth(float CurrentHealth, float MaxHealth)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(CurrentHealth / MaxHealth);
    }

    if (HealthText)
    {
        FString HealthStr = FString::Printf(TEXT("%d / %d"),
            FMath::RoundToInt(CurrentHealth), FMath::RoundToInt(MaxHealth));

        HealthText->SetText(FText::FromString(HealthStr));
    }
}

UProgressBar* UHealthUserWidget::GetHealthBar()
{
    return HealthBar;
}

UTextBlock* UHealthUserWidget::GetHealthText()
{
    return HealthText;
}
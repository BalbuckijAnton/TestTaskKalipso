// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameHUD.h"
#include "Engine/Canvas.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "HealthComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RaceGameState.h"
#include "HealthUserWidget.h"

ARaceGameHUD::ARaceGameHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarWidgetBPClass(
        TEXT("/Game/Blueprints/BP_HealthUserWidget")
    );

    if (HealthBarWidgetBPClass.Succeeded())
    {
        HealthWidgetClass = HealthBarWidgetBPClass.Class;
    }
}

void ARaceGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (HealthWidgetClass)
    {
        HealthWidget = CreateWidget<UHealthUserWidget>(GetWorld(), HealthWidgetClass);
        if (HealthWidget)
        {
            HealthWidget->AddToViewport();
        }
    }

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (PlayerPawn)
    {
        HealthComponent = PlayerPawn->FindComponentByClass<UHealthComponent>();
        if (HealthComponent)
        {
            HealthComponent->OnHealthChanged.AddDynamic(this, &ARaceGameHUD::OnHealthChanged);
            float MaxHealth = HealthComponent->GetMaxHealth();
            UpdateHealth(MaxHealth, MaxHealth);
        }
    }
}

void ARaceGameHUD::DrawHUD()
{
    Super::DrawHUD();

    ARaceGameState* GameState = GetWorld()->GetGameState<ARaceGameState>();

    if (GameState)
    {
        float TimeSec = GameState->GetElapsedTime();

        if (!FMath::IsNearlyZero(TimeSec) && Canvas && GEngine)
        {
            FString TimeStr = ConvertTimeFloatToString(TimeSec);

            FVector2D TextSize;
            Canvas->StrLen(GEngine->GetLargeFont(), TimeStr, TextSize.X, TextSize.Y);

            float X = (Canvas->SizeX - TextSize.X) / 2.0f;
            float Y = 30.0f;

            FLinearColor Color = FLinearColor::White;
            Canvas->SetLinearDrawColor(Color);
            Canvas->DrawText(GEngine->GetLargeFont(), TimeStr, X, Y, 1.0f, 1.0f);
        }
    }
}

void ARaceGameHUD::OnHealthChanged(UHealthComponent* HealthComp, float Health, float Damage)
{
    float MaxHealth = HealthComp->GetMaxHealth();
    UpdateHealth(Health, MaxHealth);
}

void ARaceGameHUD::UpdateHealth(float Health, float MaxHealth)
{
    if (!HealthWidget) return;

    if (HealthWidget)
    {
        HealthWidget->SetHealth(Health, HealthComponent->GetMaxHealth());
    }
}

FString ARaceGameHUD::ConvertTimeFloatToString(float TimeSeconds)
{
    int32 TotalMilliseconds = FMath::RoundToInt(TimeSeconds * 1000.0f);

    int32 Hours = TotalMilliseconds / (1000 * 60 * 60);
    int32 Minutes = (TotalMilliseconds / (1000 * 60)) % 60;
    int32 Seconds = (TotalMilliseconds / 1000) % 60;
    int32 Milliseconds = TotalMilliseconds % 1000;

    FString Result;

    if (Hours > 0)
    {
        Result += FString::Printf(TEXT("%d:"), Hours);
        Result += FString::Printf(TEXT("%02d:"), Minutes);
    }
    else if (Minutes > 0)
    {
        Result += FString::Printf(TEXT("%d:"), Minutes);
    }

    Result += FString::Printf(TEXT("%02d.%03d"), Seconds, Milliseconds);

    return Result;
}
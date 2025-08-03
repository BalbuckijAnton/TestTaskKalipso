// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceGameState.h"
#include "Kismet/GameplayStatics.h"

void ARaceGameState::StartRace()
{
    StartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
    FinishTime = -1.f;
}

void ARaceGameState::FinishRace()
{
    FinishTime = UGameplayStatics::GetTimeSeconds(GetWorld());
}

float ARaceGameState::GetElapsedTime() const
{
    if (StartTime >= 0 && FinishTime < 0)
    {
        return UGameplayStatics::GetTimeSeconds(GetWorld()) - StartTime;
    }
    else if (StartTime >= 0 && FinishTime >= 0)
    {
        return FinishTime - StartTime;
    }

    return 0.f;
}

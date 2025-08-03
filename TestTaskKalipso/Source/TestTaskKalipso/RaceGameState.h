// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaceGameState.generated.h"

UCLASS()
class TESTTASKKALIPSO_API ARaceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    void StartRace();
    void FinishRace();

    UFUNCTION(BlueprintCallable)
    float GetElapsedTime() const;

private:
    float StartTime = -1.f;
    float FinishTime = -1.f;
};

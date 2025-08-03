// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "RaceGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASKKALIPSO_API ARaceGameHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void DrawHUD() override;

private:
    static FString ConvertTimeFloatToString(float TimeSeconds);
};

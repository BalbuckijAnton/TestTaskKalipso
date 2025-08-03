// Fill out your copyright notice in the Description page of Project Settings.


#include "StartFinishTrigger.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "RaceGameState.h"

AStartFinishTrigger::AStartFinishTrigger()
{
}

void AStartFinishTrigger::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AStartFinishTrigger::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AStartFinishTrigger::OnOverlapEnd);
}

void AStartFinishTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        ARaceGameState* RaceState = GetWorld()->GetGameState<ARaceGameState>();
        if (!RaceState)
        {
            return;
        }

        if (IsThisStart)
        {
            RaceState->StartRace();
        }
        else
        {
            RaceState->FinishRace();
        }
    }
}

void AStartFinishTrigger::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
}

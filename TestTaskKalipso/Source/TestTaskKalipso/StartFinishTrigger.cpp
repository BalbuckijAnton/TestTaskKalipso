// Fill out your copyright notice in the Description page of Project Settings.


#include "StartFinishTrigger.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

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
        UE_LOG(LogTemp, Warning, TEXT("START or FINISH triggered by: %s"), *OtherActor->GetName());
    }
}

void AStartFinishTrigger::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor exited trigger: %s"), *OtherActor->GetName());
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowdownZone.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

namespace
{
    const TArray<FName> SpeedVarNames = { TEXT("WalkSpeeds"), TEXT("RunSpeeds"), TEXT("SprintSpeeds"), TEXT("CrouchSpeeds") };
}

ASlowdownZone::ASlowdownZone()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ASlowdownZone::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ASlowdownZone::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ASlowdownZone::OnOverlapEnd);
}

void ASlowdownZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        TMap<FName, FVector> OriginalMap;

        for (const auto& SpeedVarName : SpeedVarNames)
        {
            FProperty* Prop = OtherActor->GetClass()->FindPropertyByName(SpeedVarName);
            if (FStructProperty* StructProp = CastField<FStructProperty>(Prop))
            {
                if (StructProp->Struct == TBaseStructure<FVector>::Get())
                {
                    void* ValuePtr = StructProp->ContainerPtrToValuePtr<void>(OtherActor);
                    FVector* SpeedPtr = static_cast<FVector*>(ValuePtr);

                    if (SpeedPtr)
                    {
                        OriginalMap.Add(SpeedVarName, *SpeedPtr);
                        // TODO: Multiplyer to UPROPERTY
                        *SpeedPtr = (*SpeedPtr) / 2;
                    }
                }
            }
        }

        OriginalSpeeds.Add(OtherActor, std::move(OriginalMap));
    }
}

void ASlowdownZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || !OriginalSpeeds.Contains(OtherActor)) return;

    TMap<FName, FVector> OriginalMap = OriginalSpeeds[OtherActor];

    for (const auto& SpeedVarName : SpeedVarNames)
    {
        FProperty* Prop = OtherActor->GetClass()->FindPropertyByName(SpeedVarName);
        if (FStructProperty* StructProp = CastField<FStructProperty>(Prop))
        {
            if (StructProp->Struct == TBaseStructure<FVector>::Get())
            {
                void* ValuePtr = StructProp->ContainerPtrToValuePtr<void>(OtherActor);
                FVector* SpeedPtr = static_cast<FVector*>(ValuePtr);

                if (SpeedPtr)
                {
                    *SpeedPtr = OriginalMap[SpeedVarName];
                }
            }
        }
    }

    OriginalSpeeds.Remove(OtherActor);
}

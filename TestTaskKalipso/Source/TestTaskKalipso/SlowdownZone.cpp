#include "SlowdownZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UnrealType.h"

namespace
{
    const TArray<FName> SpeedVarNames = { TEXT("WalkSpeeds"), TEXT("RunSpeeds"), TEXT("SprintSpeeds"), TEXT("CrouchSpeeds") };
}

ASlowdownZone::ASlowdownZone()
{
    PrimaryActorTick.bCanEverTick = false;

    // Trigger Box
    TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
    TriggerZone->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerZone->SetCollisionObjectType(ECC_WorldDynamic);
    TriggerZone->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerZone->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerZone->SetBoxExtent(FVector(200.f, 200.f, 100.f));
    RootComponent = TriggerZone;

    // Mesh
    ZoneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ZoneMesh"));
    ZoneMesh->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMesh.Succeeded())
    {
        ZoneMesh->SetStaticMesh(CubeMesh.Object);
        ZoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    ZoneMesh->SetRelativeScale3D(TriggerZone->GetUnscaledBoxExtent() * 2 / 100.f); // cube mesh = 100 units
}

void ASlowdownZone::BeginPlay()
{
    Super::BeginPlay();

    FVector Extent = TriggerZone->GetUnscaledBoxExtent(); // half of size
    ZoneMesh->SetRelativeLocation(FVector(0.f, 0.f, -Extent.Z));
    Extent.Z /= 2.0f;
    ZoneMesh->SetRelativeScale3D(Extent * 2.f / 100.f); // cube = 100 units
    

    if (ZoneMaterial)
    {
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(ZoneMaterial, this);
        ZoneMesh->SetMaterial(0, DynMat);

        // TODO: Not worked params
        DynMat->SetScalarParameterValue(TEXT("Glow"), 1.0f);
        DynMat->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor(1.0f, 0.0f, 0.0f));
        DynMat->SetScalarParameterValue(TEXT("Opacity"), 0.5f);
    }

    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ASlowdownZone::OnOverlapBegin);
    TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ASlowdownZone::OnOverlapEnd);
}

void ASlowdownZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    TMap<FName, FVector> OriginalMap;

    for (const auto& SpeedVarName : SpeedVarNames)
    {
        FProperty* Prop = OtherActor->GetClass()->FindPropertyByName(SpeedVarName);
        if (FStructProperty* StructProp = CastField<FStructProperty>(Prop))
        {
            if (StructProp->Struct == TBaseStructure<FVector>::Get())
            {
                void* ValuePtr = StructProp->ContainerPtrToValuePtr<void>(OtherActor);
                if (FVector* SpeedPtr = static_cast<FVector*>(ValuePtr))
                {
                    OriginalMap.Add(SpeedVarName, *SpeedPtr);
                    *SpeedPtr = (*SpeedPtr) * SlowdownMultiplier;
                }
            }
        }
    }

    if (OriginalMap.Num() > 0)
    {
        OriginalSpeeds.Add(OtherActor, MoveTemp(OriginalMap));
    }
}

void ASlowdownZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || !OriginalSpeeds.Contains(OtherActor)) return;

    const TMap<FName, FVector>& OriginalMap = OriginalSpeeds[OtherActor];

    for (const auto& SpeedVarName : SpeedVarNames)
    {
        FProperty* Prop = OtherActor->GetClass()->FindPropertyByName(SpeedVarName);
        if (FStructProperty* StructProp = CastField<FStructProperty>(Prop))
        {
            if (StructProp->Struct == TBaseStructure<FVector>::Get())
            {
                void* ValuePtr = StructProp->ContainerPtrToValuePtr<void>(OtherActor);
                if (FVector* SpeedPtr = static_cast<FVector*>(ValuePtr))
                {
                    *SpeedPtr = OriginalMap[SpeedVarName];
                }
            }
        }
    }

    OriginalSpeeds.Remove(OtherActor);
}
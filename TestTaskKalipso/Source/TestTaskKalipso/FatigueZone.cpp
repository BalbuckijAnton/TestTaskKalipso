#include "FatigueZone.h"
#include "Components/BoxComponent.h"
#include "StaminaComponent.h"


AFatigueZone::AFatigueZone()
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

void AFatigueZone::BeginPlay()
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

    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &AFatigueZone::OnOverlapBegin);
    TriggerZone->OnComponentEndOverlap.AddDynamic(this, &AFatigueZone::OnOverlapEnd);
}

void AFatigueZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || OldStamina.Contains(OtherActor)) return;

    UStaminaComponent* StaminaComponent = Cast<UStaminaComponent>(OtherActor->GetComponentByClass(UStaminaComponent::StaticClass()));
    if (StaminaComponent)
    {
        float OldStaminaVal = StaminaComponent->SetMaxStamina(StaminaComponent->GetMaxStamina() / FatigueMultiplier);
        OldStamina.Add(OtherActor, OldStaminaVal);
    }
}

void AFatigueZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor || !OldStamina.Contains(OtherActor)) return;

    float OldStaminaVal = OldStamina.FindAndRemoveChecked(OtherActor);
    UStaminaComponent* StaminaComponent = Cast<UStaminaComponent>(OtherActor->GetComponentByClass(UStaminaComponent::StaticClass()));
    if (StaminaComponent)
    {
        StaminaComponent->SetMaxStamina(OldStaminaVal);
    }
}
#include "DamageZone.h"
#include "Components/BoxComponent.h"
#include "HealthComponent.h"

ADamageZone::ADamageZone()
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

void ADamageZone::BeginPlay()
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

    TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlapBegin);
    TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ADamageZone::OnOverlapEnd);
}

void ADamageZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this || ActiveTimers.Contains(OtherActor)) return;

    UHealthComponent* HealthComponent = Cast< UHealthComponent>(OtherActor->GetComponentByClass(UHealthComponent::StaticClass()));
    if (HealthComponent)
    {
        FTimerHandle NewTimer;
        GetWorldTimerManager().SetTimer(
            NewTimer,
            [this, HealthComponent]()
            {
                ApplyDamageToActor(HealthComponent);
            },
            DamageIntervalSecond,
            true
        );

        ActiveTimers.Add(OtherActor, NewTimer);
    }
}

void ADamageZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (!OtherActor) return;

    if (FTimerHandle* Timer = ActiveTimers.Find(OtherActor))
    {
        GetWorldTimerManager().ClearTimer(*Timer);
        ActiveTimers.Remove(OtherActor);
    }
}

void ADamageZone::ApplyDamageToActor(UHealthComponent* ActorHealthComponent)
{
    if (!ActorHealthComponent)
        return;

    ActorHealthComponent->Damage(DamagePerSecond * DamageIntervalSecond);
}
#include "DamageZone.h"
#include "HealthComponent.h"

ADamageZone::ADamageZone()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ADamageZone::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &ADamageZone::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ADamageZone::OnOverlapEnd);
}

void ADamageZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
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

void ADamageZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
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
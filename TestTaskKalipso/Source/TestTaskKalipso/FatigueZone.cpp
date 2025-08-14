#include "FatigueZone.h"
#include "StaminaComponent.h"


AFatigueZone::AFatigueZone()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AFatigueZone::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AFatigueZone::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &AFatigueZone::OnOverlapEnd);
}

void AFatigueZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || OtherActor == this || OldStamina.Contains(OtherActor)) return;

    UStaminaComponent* StaminaComponent = Cast<UStaminaComponent>(OtherActor->GetComponentByClass(UStaminaComponent::StaticClass()));
    if (StaminaComponent)
    {
        float OldStaminaVal = StaminaComponent->SetMaxStamina(StaminaComponent->GetMaxStamina() / FatigueMultiplier);
        OldStamina.Add(OtherActor, OldStaminaVal);
    }
}

void AFatigueZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || !OldStamina.Contains(OtherActor)) return;

    float OldStaminaVal = OldStamina.FindAndRemoveChecked(OtherActor);
    UStaminaComponent* StaminaComponent = Cast<UStaminaComponent>(OtherActor->GetComponentByClass(UStaminaComponent::StaticClass()));
    if (StaminaComponent)
    {
        StaminaComponent->SetMaxStamina(OldStaminaVal);
    }
}
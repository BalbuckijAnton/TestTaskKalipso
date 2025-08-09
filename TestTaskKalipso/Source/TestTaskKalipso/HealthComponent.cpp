#include "HealthComponent.h"
#include "GameFramework/Actor.h"

UHealthComponent::UHealthComponent()
{
    DefaultHealth = 100.0f;
    Health = DefaultHealth;
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = DefaultHealth;

    // ѕодписка на событие получени€ урона
    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeAnyDamage);
    }
}

void UHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage,
    const class UDamageType* DamageType,
    class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || Health <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

    OnHealthChanged.Broadcast(this, Health, Damage);

    // «десь можно добавить дополнительную логику: смерть, анимации и т.п.
}

float UHealthComponent::GetMaxHealth() const
{
    return DefaultHealth;
}

float UHealthComponent::GetHealth() const
{
    return Health;
}

void UHealthComponent::Heal(float HealAmount)
{
    if (HealAmount <= 0.0f || Health <= 0.0f)
    {
        return;
    }

    Health = FMath::Clamp(Health + HealAmount, 0.0f, DefaultHealth);

    OnHealthChanged.Broadcast(this, Health, -HealAmount); // ќтрицательное значение дл€ обозначени€ лечени€
}
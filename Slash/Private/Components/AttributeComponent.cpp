// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Constructor
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Begin Play
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

/** Damage */
void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
}

/** Health */
float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}

void UAttributeComponent::RegenHealth(float DeltaTime)
{
	Health = FMath::Clamp(Health + HealthRegenRate * DeltaTime, 0.f, MaxHealth);
}

void UAttributeComponent::SetHealth(float NewHealth)
{
	Health = NewHealth;
}

/** Stamina */
void UAttributeComponent::UseStamina(float StaminaCost)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0, MaxStamina);
}

float UAttributeComponent::GetStaminaPercent()
{
	return Stamina / MaxStamina;
}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

/** Is Function */
bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

// Tick
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

/** Add */
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

void UAttributeComponent::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}
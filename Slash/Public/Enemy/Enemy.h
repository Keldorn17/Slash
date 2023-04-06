// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	virtual void Die() override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual int32 PlayDeathMontage() override;
	/** </ABaseCharacter> */

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose = EDeathPose::EDP_Death1;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
private:
	/** AI Behavior */
	void SpawnDefaultWeapon();
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPanSensingComponent

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensing;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY()
	TObjectPtr<class AAIController> EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = .1f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = .2f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrolingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = .5f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 8.f;

};

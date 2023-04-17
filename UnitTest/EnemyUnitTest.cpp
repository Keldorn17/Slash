#include "Enemy.h"
#include "Engine.h"
#include "Tests/AutomationCommon.h"
#include "AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnemyUnitTest, "Game.EnemyUnitTests", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool FEnemyUnitTest::RunTest(const FString& Parameters)
{
	// Create a WorldContext for running the test
	FAutomationTestFramework::Get().InitializeWorld(true);

	// Spawn an instance of AEnemy
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	AEnemy* Enemy = Cast<AEnemy>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AEnemy::StaticClass(), FTransform(SpawnRotation, SpawnLocation)));
	if (Enemy)
	{
		UGameplayStatics::FinishSpawningActor(Enemy, FTransform(SpawnRotation, SpawnLocation));

		// Test initial enemy state
		TestTrue("Enemy is initially alive", !Enemy->IsDead());
		TestEqual("Enemy's initial state is Patrolling", Enemy->GetEnemyState(), EEnemyState::EES_Patrolling);
		TestFalse("Enemy is not initially engaged", Enemy->IsEngaged());
		TestFalse("Enemy is not initially chasing", Enemy->IsChasing());

		// Test patrol target selection
		AActor* PatrolTarget = Enemy->ChoosePatrolTarget();
		TestNotNull("Patrol target is valid", PatrolTarget);
		if (PatrolTarget)
		{
			// Test patrol target is within patrol radius
			float PatrolRadius = Enemy->GetPatrolRadius();
			float DistanceToPatrolTarget = (PatrolTarget->GetActorLocation() - Enemy->GetActorLocation()).Size();
			TestTrue("Patrol target is within patrol radius", DistanceToPatrolTarget <= PatrolRadius);
		}

		// Test combat target selection
		AActor* CombatTarget = Enemy->GetCombatTarget();
		TestNull("Combat target is null initially", CombatTarget);
		
		// Test chasing behavior
		Enemy->ChaseTarget();
		TestEqual("Enemy state is changed to Chasing", Enemy->GetEnemyState(), EEnemyState::EES_Chasing);
		TestTrue("Enemy is now chasing", Enemy->IsChasing());
		
		// Test engagement behavior
		Enemy->SetEngaged(true);
		TestTrue("Enemy is now engaged", Enemy->IsEngaged());

		// Test attack behavior
		Enemy->StartAttackTimer();
		TestTrue("Attack timer is started", Enemy->IsAttackTimerActive());
		Enemy->ClearAttackTimer();
		TestFalse("Attack timer is cleared", Enemy->IsAttackTimerActive());
		
		// Test death behavior
		Enemy->Die();
		TestTrue("Enemy is now dead", Enemy->IsDead());
		TestEqual("Enemy state is changed to Dead", Enemy->GetEnemyState(), EEnemyState::EES_Dead);
	}

	// Clean up the spawned enemy
	if (Enemy)
	{
		Enemy->Destroy();
	}

	// Tear down the WorldContext
	FAutomationTestFramework::Get().TearDownWorld();

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS

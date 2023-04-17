#include "Tests/AutomationCommon.h"
#include "Characters/SlashCharacter.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ASlashCharacterTest, "Game.CharacterTests.ASlashCharacter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::SmokeFilter)

bool ASlashCharacterTest::RunTest(const FString& Parameters)
{
    // Create a world for testing
    FAutomationTestWorld TestWorld = FAutomationTestWorld();
    TestWorld.Initialize();

    // Create an instance of ASlashCharacter
    ASlashCharacter* SlashCharacter = TestWorld.SpawnActor<ASlashCharacter>();

    // Test if ASlashCharacter is valid
    TestNotNull("ASlashCharacter is valid", SlashCharacter);

    // Test ASlashCharacter's default values
    TestTrue("ASlashCharacter has default values", SlashCharacter->IsAlive() && SlashCharacter->Attributes != nullptr && SlashCharacter->SlashOverlay == nullptr);

    // Test ASlashCharacter's SetupPlayerInputComponent() function
    UInputComponent* InputComponent = NewObject<UInputComponent>();
    SlashCharacter->SetupPlayerInputComponent(InputComponent);
    TArray<FInputActionBinding> ActionBindings = InputComponent->GetActionBindings();
    bool bBindingsFound = false;
    for (const FInputActionBinding& Binding : ActionBindings)
    {
        if (Binding.ActionName == ASlashCharacter::MovementAction ||
            Binding.ActionName == ASlashCharacter::LookAction ||
            Binding.ActionName == ASlashCharacter::JumpAction ||
            Binding.ActionName == ASlashCharacter::EKeyAction ||
            Binding.ActionName == ASlashCharacter::AttackAction ||
            Binding.ActionName == ASlashCharacter::DodgeAction ||
            Binding.ActionName == ASlashCharacter::MenuKeyAction)
        {
            bBindingsFound = true;
            break;
        }
    }
    TestTrue("ASlashCharacter's SetupPlayerInputComponent() binds actions correctly", bBindingsFound);

    // Clean up
    TestWorld.DestroyActor(SlashCharacter);

    // End the test
    TestWorld.Teardown();
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS

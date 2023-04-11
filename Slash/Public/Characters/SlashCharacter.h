// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class AItems;
class ASoul;
class ATreasure;
class UAnimMontage;
class USlashOverlay;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ASlashCharacter();

	/** <AActor> */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	/** </AActor> */

	/** <ACharacter> */
	virtual void Jump() override;
	/** </ACharacter> */

	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

	/** <IPickupInterface> */
	virtual void SetOverlappingItem(class AItems* Item) override;
	virtual void AddSouls(class ASoul* Soul) override;
	virtual void AddGold(class ATreasure* Treasure) override;
	/** </IPickupInterface> */

protected:
	virtual void BeginPlay() override;

	/** Callbacks for input */
	void EKeyPressed();
	virtual void Attack() override;
	void Dodge();

	// Enhanced Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void MenuKeyPressed();

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> SlashContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> EKeyAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> DodgeAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MenuKeyAction;

	/** HUD */
	UFUNCTION(BlueprintCallable)
	void InitializeSlashOverlay();

	void CreateMenu(APlayerController* PlayerController);
	void CloseMenu(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void CreateControlls();

	UFUNCTION(BlueprintCallable)
	void CreateSettings();

	UFUNCTION(BlueprintCallable)
	void CloseControlls();

	UFUNCTION(BlueprintCallable)
	void CloseSettings();

	UPROPERTY(EditAnywhere, Category = Input)
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MenuWidget;

	UPROPERTY(EditAnywhere, Category = Input)
	TSubclassOf<UUserWidget> StartMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> StartMenuWidget;

	UPROPERTY(EditAnywhere, Category = Input)
	TSubclassOf<UUserWidget> SettingsWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> SettingsWidget;

	UPROPERTY(EditAnywhere, Category = Input)
	TSubclassOf<UUserWidget> ControllsWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> ControllsWidget;

	UPROPERTY(BlueprintReadWrite)
	bool IsMenuOpen = false;

	UPROPERTY(BlueprintReadWrite)
	bool bStartMenuClosed = false;

	UPROPERTY(BlueprintReadWrite)
	bool bInSettings = false;

	UPROPERTY(BlueprintReadWrite)
	bool bInControlls = false;

	/** Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual void DodgeEnd() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	void PlayEquipMontage(const FName SectionName);
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

private:
	/** Helper Functions */
	bool IsUnoccupied();
	bool HasEnoughStamina();

	/** HUD */
	void SetHUDHealth();
	void ShowStartMenu();

	/** Character components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	TObjectPtr<UGroomComponent> Eyebrows;
	
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItems> OverlappingItem;

	/** Montage */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	TObjectPtr<UAnimMontage> EquipMontage;

	/** States */
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/** HUD */
	UPROPERTY()
	TObjectPtr<USlashOverlay> SlashOverlay;

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
};

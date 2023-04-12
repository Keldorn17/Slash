// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Items.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Animation/AnimMontage.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Constructor
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

// Begin Play
void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}

	Tags.Add(FName("EngageableTarget"));
	ShowStartMenu();

	SpawnTransform = GetActorTransform();
}

// Tick
void ASlashCharacter::Tick(float DeltaTime)
{
	if (IsAlive() && Attributes)
	{
		if (Attributes->GetStaminaPercent() < 1.f && SlashOverlay)
		{
			Attributes->RegenStamina(DeltaTime);
			SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
		}
		if (Attributes->GetHealthPercent() < 1.f && SlashOverlay)
		{
			Attributes->RegenHealth(DeltaTime);
			SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
		}
	}
}

// Setup Input Component
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
		EnhancedInputComponent->BindAction(MenuKeyAction, ETriggerEvent::Started, this, &ASlashCharacter::MenuKeyPressed);
	}
}

/**
* HUD
*/

void ASlashCharacter::InitializeSlashOverlay()
{
	bStartMenuClosed = true;
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			if (SlashOverlay && Attributes)
			{
				SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
				SlashOverlay->SetStaminaBarPercent(1.f);
				SlashOverlay->SetGold(0);
				SlashOverlay->SetSouls(0);
			}
		}
	}
}

void ASlashCharacter::ShowStartMenu()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (World)
	{
		if (StartMenuWidgetClass && PlayerController)
		{
			if (!IsMenuOpen)
			{
				IsMenuOpen = true;
				StartMenuWidget = CreateWidget<UUserWidget>(PlayerController, StartMenuWidgetClass);
				StartMenuWidget->AddToViewport();
				PlayerController->bShowMouseCursor = true;
				UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, StartMenuWidget);
				UGameplayStatics::SetGamePaused(World, true);
			}
		}
	}
}

void ASlashCharacter::SetHUDHealth()
{
	if (SlashOverlay && Attributes)
	{
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
}

/** Create Widget */
void ASlashCharacter::CreateMenu()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (World && MenuWidgetClass && PlayerController)
	{
		IsMenuOpen = true;
		MenuWidget = CreateWidget<UUserWidget>(PlayerController, MenuWidgetClass);
		MenuWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PlayerController, MenuWidget);
		UGameplayStatics::SetGamePaused(World, true);
	}
}

void ASlashCharacter::CreateControlls()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (ControllsWidgetClass && PlayerController && MenuWidget)
	{
		ControllsWidget = CreateWidget<UUserWidget>(PlayerController, ControllsWidgetClass);
		ControllsWidget->AddToViewport();
		MenuWidget->RemoveFromParent();
		bInControlls = true;
	}
}

void ASlashCharacter::CreateSettings()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (SettingsWidgetClass && PlayerController && MenuWidget)
	{
		SettingsWidget = CreateWidget<UUserWidget>(PlayerController, SettingsWidgetClass);
		SettingsWidget->AddToViewport();
		MenuWidget->RemoveFromParent();
		bInSettings = true;
	}
}

/** Close Widget */
void ASlashCharacter::CloseMenu()
{
	UWorld* World = GetWorld();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (World && MenuWidget && PlayerController)
	{
		IsMenuOpen = false;
		MenuWidget->RemoveFromParent();
		PlayerController->bShowMouseCursor = false;
		UGameplayStatics::SetGamePaused(World, false);
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}
}

void ASlashCharacter::CloseControlls()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && MenuWidgetClass && ControllsWidget)
	{
		ControllsWidget->RemoveFromParent();
		MenuWidget = CreateWidget<UUserWidget>(PlayerController, MenuWidgetClass);
		MenuWidget->AddToViewport();
		bInControlls = false;
	}
}

void ASlashCharacter::CloseSettings()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && MenuWidgetClass && SettingsWidget)
	{
		SettingsWidget->RemoveFromParent();
		MenuWidget = CreateWidget<UUserWidget>(PlayerController, MenuWidgetClass);
		MenuWidget->AddToViewport();
		bInSettings = false;
	}
}

/**
* Menu Key Action
*/

void ASlashCharacter::MenuKeyPressed()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController && bStartMenuClosed)
	{
		if (!IsMenuOpen)
		{
			CreateMenu();
		}
		else if (!bInSettings && !bInControlls)
		{
			CloseMenu();
		}
		else if (bInSettings)
		{
			CloseSettings();
		}
		else if (bInControlls)
		{
			CloseControlls();
		}
	}
}

/**
* Weapon Key Action
*/

void ASlashCharacter::EKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			EquippedWeapon->Destroy();
		}
		EquipWeapon(OverlappingWeapon);
	}
	else 
	{
		if (CanDisarm())
		{
			Disarm();
		}
		else if (CanArm())
		{
			Arm();
		}
	}
}

/**
* Combat
*/

void ASlashCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void ASlashCharacter::Disarm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Die_Implementation()
{
	Super::Die_Implementation();

	ActionState = EActionState::EAS_Dead;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASlashCharacter::Respawn, RespawnTime);
}

/**
* Hit
*/

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f)
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/**
* Damage
*/

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

/**
* Pickup
*/

void ASlashCharacter::SetOverlappingItem(AItems* Item)
{
	OverlappingItem = Item;
}

void ASlashCharacter::AddSouls(ASoul* Soul)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddSouls(Soul->GetSouls());
		SlashOverlay->SetSouls(Attributes->GetSouls());
	}
}

void ASlashCharacter::AddGold(ATreasure* Treasure)
{
	if (Attributes && SlashOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		SlashOverlay->SetGold(Attributes->GetGold());
	}
}

/**
* Weapon Enum Conditions
*/

void ASlashCharacter::Dodge()
{
	if (!IsUnoccupied() || !HasEnoughStamina()) return;
	
	PlayDodgeMontage();
	ActionState = EActionState::EAS_Dodge;
	if (Attributes && SlashOverlay)
	{
		Attributes->UseStamina(Attributes->GetDodgeCose());
		SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
	}
}

void ASlashCharacter::DodgeEnd()
{
	Super::DodgeEnd();

	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

/**
* Weapon Attachment
*/

void ASlashCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

/**
* Montage Functions
*/

void ASlashCharacter::PlayEquipMontage(const FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

/**
* Movement Key Actions
*/

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::Jump()
{
	if (IsUnoccupied())
	{
		Super::Jump();
	}
}

/**
* Helper Functions
*/

bool ASlashCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::HasEnoughStamina()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCose();
}

void ASlashCharacter::Respawn()
{
	Tags.Remove(FName("Dead"));
	ActionState = EActionState::EAS_Unoccupied;
	EnableCapsule();
	EnableMeshCollision();
	SetActorTransform(SpawnTransform);
	GetWorldTimerManager().ClearTimer(RespawnTimer);

	if (Attributes && SlashOverlay)
	{
		Attributes->SetHealth(Attributes->GetMaxHealth());
		SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
	}
	}
}
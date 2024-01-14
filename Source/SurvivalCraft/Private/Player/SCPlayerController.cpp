// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Player/SCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "BuildingSystem/SCBuildingComponent.h"
#include "Character/SCCharacter.h"
#include "Enums/PlayerStats.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Data/SCItemData.h"

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ASCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ThisClass::ToggleInventory);
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Started, this, &ThisClass::OnLeftMouse);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComponent->BindAction(BuildAction, ETriggerEvent::Started, this, &ThisClass::Build);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASCPlayerController::ClientUpdateItemSlot_Implementation(EContainerType ContainerType, int32 SlotIndex, const FItemInformation& Item)
{
	OnUpdateItemSlotDelegate.Broadcast(ContainerType, SlotIndex, Item);
}

void ASCPlayerController::ClientResetItemSlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	OnResetItemSlotDelegate.Broadcast(ContainerType, SlotIndex);
}

void ASCPlayerController::ClientUpdateArmorSlot_Implementation(EArmorType ArmorType, const FItemInformation& Item)
{
	OnUpdateArmorSlotDelegate.Broadcast(ArmorType, Item);
}

void ASCPlayerController::ClientResetArmorSlot_Implementation(EArmorType ArmorType)
{
	OnResetArmorSlotDelegate.Broadcast(ArmorType);
}

void ASCPlayerController::ShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName)
{
	OnItemAddedDelegate.Broadcast(ItemIcon, ItemQuantity, ItemName);
}

bool ASCPlayerController::CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	return GetSCCharacter()->CanCraftItem(ItemID, ContainerType, TableType);
}

void ASCPlayerController::CraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	return GetSCCharacter()->ServerCraftItem(ItemID, ContainerType, TableType);
}

void ASCPlayerController::UpdatePlayerStats(EPlayerStats PlayerStats, float NewValue)
{
	switch (PlayerStats)
	{
		case EPlayerStats::EPS_Health:
			OnPlayerHealthChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxHealth:
			OnPlayerMaxHealthChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_Food:
			OnPlayerFoodChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxFood:
			OnPlayerMaxFoodChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_Water:
			OnPlayerWaterChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxWater:
			OnPlayerMaxWaterChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_Stamina:
			OnPlayerStaminaChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxStamina:
			OnPlayerMaxStaminaChangedDelegate.Broadcast(NewValue);
			break;
	}
}

void ASCPlayerController::UpdatePlayerWindow(UMaterialInstanceDynamic* Material)
{
	OnPlayerWindowInitDelegate.Broadcast(Material);
}

float ASCPlayerController::GetHealth()
{
	return GetSCCharacter()->GetHealth();
}

float ASCPlayerController::GetMaxHealth()
{
	return GetSCCharacter()->GetMaxHealth();
}

float ASCPlayerController::GetFood()
{
	return GetSCCharacter()->GetFood();
}

float ASCPlayerController::GetMaxFood()
{
	return GetSCCharacter()->GetMaxFood();
}

float ASCPlayerController::GetWater()
{
	return GetSCCharacter()->GetWater();
}

float ASCPlayerController::GetMaxWater()
{
	return GetSCCharacter()->GetMaxWater();
}

float ASCPlayerController::GetStamina()
{
	return GetSCCharacter()->GetStamina();
}

float ASCPlayerController::GetMaxStamina()
{
	return GetSCCharacter()->GetMaxStamina();
}

void ASCPlayerController::ServerSpendSkillPoint_Implementation(EPlayerStats StatToUpgrade)
{
	IPlayerInterface::Execute_SpendSkillPoint(GetSCCharacter(), StatToUpgrade);
}

void ASCPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
	}
}

void ASCPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASCPlayerController::Jump()
{
	GetCharacter()->Jump();
}

void ASCPlayerController::StopJump()
{
	GetCharacter()->StopJumping();
}

void ASCPlayerController::ToggleInventory()
{
	OnToggleInventoryDelegate.Broadcast();
}

void ASCPlayerController::OnLeftMouse()
{
	// TODO: Create a interface for the Character so I don't need to cast.
	GetSCCharacter()->UseEquipable();
}

void ASCPlayerController::Interact()
{
	GetSCCharacter()->Interact();
}

void ASCPlayerController::Sprint()
{
	ASCCharacter* Char = GetSCCharacter();

	// TODO: Need to refactor this so we can have a sprint with prediction
	Char->GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	Char->ServerSprint(true);
}

void ASCPlayerController::StopSprint()
{
	ASCCharacter* Char = GetSCCharacter();

	Char->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	Char->ServerSprint(false);
}

void ASCPlayerController::Build()
{
	int32 RandomNumber = FMath::RandRange(1, 2);
	GetSCCharacter()->StructureID = RandomNumber;
	GetSCCharacter()->GetBuildingComponent()->ClientLaunchBuildMode(RandomNumber);
}

ASCCharacter* ASCPlayerController::GetSCCharacter()
{
	return SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetCharacter()) : SCCharacter;
}

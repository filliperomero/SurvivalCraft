// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Player/SCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/SCCharacter.h"
#include "GameFramework/Character.h"
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

void ASCPlayerController::ShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName)
{
	OnItemAddedDelegate.Broadcast(ItemIcon, ItemQuantity, ItemName);
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
	SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetCharacter()) : SCCharacter;

	// TODO: Create a interface for the Character so I don't need to cast.
	SCCharacter->UseEquipable();
}

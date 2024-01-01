﻿// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Crafting/Data/CraftingData.h"
#include "Enums/ContainerType.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

enum class EPlayerStats : uint8;
class ASCCharacter;
struct FItemInformation;
DECLARE_MULTICAST_DELEGATE(FOnToggleInventorySignature)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemSlotSignature, EContainerType/*ContainerType*/, int32/*SlotIndex*/, const FItemInformation&/*Item*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnResetItemSlotSignature, EContainerType/*ContainerType*/, int32/*SlotIndex*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedSignature, UTexture2D*/*ItemIcon*/, int32/*ItemQuantity*/, FText/*ItemName*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsChangedSignature, float/*NewValue*/);

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SURVIVALCRAFT_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void ClientUpdateItemSlot(EContainerType ContainerType, int32 SlotIndex, const FItemInformation& Item);
	
	UFUNCTION(Client, Reliable)
	void ClientResetItemSlot(EContainerType ContainerType, int32 SlotIndex);
	
	void ShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName);
	bool CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);
	void CraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);
	void UpdatePlayerStats(EPlayerStats PlayerStats, float NewValue);
	float GetHealth();
	float GetMaxHealth();
	float GetFood();
	float GetMaxFood();
	float GetWater();
	float GetMaxWater();
	
	FOnToggleInventorySignature OnToggleInventoryDelegate;
	FOnUpdateItemSlotSignature OnUpdateItemSlotDelegate;
	FOnResetItemSlotSignature OnResetItemSlotDelegate;
	FOnItemAddedSignature OnItemAddedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerHealthChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerFoodChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerWaterChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerStaminaChangedDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	ASCCharacter* GetSCCharacter();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	void ToggleInventory();
	void OnLeftMouse();
	void Interact();

	UPROPERTY()
	ASCCharacter* SCCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;
};

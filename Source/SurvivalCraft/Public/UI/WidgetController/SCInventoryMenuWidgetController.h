// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "Items/Data/SCItemData.h"
#include "SCInventoryMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemSlotWidgetSignature, EContainerType, ContainerType, int32, SlotIndex, const FItemInformation&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResetItemSlotWidgetSignature, EContainerType, ContainerType, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateArmorSlotWidgetSignature, EArmorType, ArmorType, const FItemInformation&, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResetArmorSlotWidgetSignature, EArmorType, ArmorType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerWindowWidgetSignature, UMaterialInstanceDynamic*, Material);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnToggleStorageWidgetSignature, int32, TotalSlots, ECraftingType, StorageType, bool, bCloseStorage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateStorageWidgetSignature, int32, TotalSlots);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCInventoryMenuWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateItemSlotWidgetSignature OnUpdateItemSlotWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnResetItemSlotWidgetSignature OnResetItemSlotWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatsChangedSignature OnPlayerLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateArmorSlotWidgetSignature OnUpdateArmorSlotWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnResetArmorSlotWidgetSignature OnResetArmorSlotWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerWindowWidgetSignature OnPlayerWindowWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnToggleStorageWidgetSignature OnToggleStorageWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnToggleInventoryWidgetSignature OnToggleInventoryWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateStorageWidgetSignature OnUpdateStorageWidgetDelegate;

	UFUNCTION(BlueprintCallable)
	void CharacterCloseStorage();

	UFUNCTION(BlueprintCallable)
	void ActionButtonPressed();

	UFUNCTION(BlueprintCallable)
	void ShowItemOptionsMenu(int32 Index, EContainerType Container);

	UFUNCTION(BlueprintCallable)
	void HideItemOptionsMenu();
};

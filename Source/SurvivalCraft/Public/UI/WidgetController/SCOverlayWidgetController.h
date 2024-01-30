// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCOverlayWidgetController.generated.h"

enum class EContainerType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedWidgetSignature, UTexture2D*, ItemIcon, int32, ItemQuantity, FText, ItemName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDemolishStructureWidgetSignature, bool, bCancelDemolish, float, DemolishTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShowItemOptionsWidgetSignature, int32, Index, EContainerType, Container);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHideItemOptionsWidgetSignature);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCOverlayWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnToggleInventoryWidgetSignature OnToggleInventoryWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnItemAddedWidgetSignature OnItemAddedWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatsChangedSignature OnXPChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatsChangedSignature OnLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnDemolishStructureWidgetSignature OnDemolishStructureWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnShowItemOptionsWidgetSignature OnShowItemOptionsWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnHideItemOptionsWidgetSignature OnHideItemOptionsWidgetDelegate;

	UFUNCTION(BlueprintCallable)
	void HideItemOptionsMenu();

	UFUNCTION(BlueprintCallable)
	void DropItem(EContainerType ContainerType, int32 FromIndex);
};

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCOverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToggleInventoryWidgetSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedWidgetSignature, UTexture2D*, ItemIcon, int32, ItemQuantity, FText, ItemName);

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
};

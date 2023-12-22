// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCInventoryMenuWidgetController.h"
#include "SCWidgetController.h"
#include "SCHotbarMenuWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCHotbarMenuWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateItemSlotWidgetSignature OnUpdateItemSlotWidgetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnResetItemSlotWidgetSignature OnResetItemSlotWidgetDelegate;
};

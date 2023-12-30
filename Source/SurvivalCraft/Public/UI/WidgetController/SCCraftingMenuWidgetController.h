// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCCraftingMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCraftingItemSelectedSignature, const int32, ItemID, const EContainerType, ContainerType);

enum class EContainerType : uint8;

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCCraftingMenuWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CraftingItemSelected(const int32 ItemID, const EContainerType ContainerType);

	UPROPERTY(BlueprintAssignable)
	FOnCraftingItemSelectedSignature OnCraftingItemSelectedDelegate;
};

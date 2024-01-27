// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "Crafting/Data/CraftingData.h"
#include "SCCraftingMenuWidgetController.generated.h"

enum class EContainerType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCraftingItemSelectedSignature, const int32, ItemID, const EContainerType, ContainerType, const bool, bCanCraft, const ECraftingType, CraftingType);

struct FSelectedItem
{
	int32 ItemID = 0;

	EContainerType ContainerType;

	ECraftingType TableType;
};

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCCraftingMenuWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CraftingItemSelected(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);

	UFUNCTION(BlueprintCallable)
	void CraftButtonPressed();

	UPROPERTY(BlueprintAssignable)
	FOnCraftingItemSelectedSignature OnCraftingItemSelectedDelegate;

private:
	FSelectedItem SelectedCraftingItem;
};

// Copyright Fillipe Romero

#include "UI/WidgetController/SCCraftingMenuWidgetController.h"
#include "Enums/ContainerType.h"
#include "Player/SCPlayerController.h"

void USCCraftingMenuWidgetController::CraftingItemSelected(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	const bool bCanCraft = GetSCPC()->CanCraftItem(ItemID, ContainerType, TableType);

	SelectedCraftingItem= { ItemID, ContainerType, TableType };

	OnCraftingItemSelectedDelegate.Broadcast(ItemID, ContainerType, bCanCraft, TableType);
}

void USCCraftingMenuWidgetController::CraftButtonPressed()
{
	if (GetSCPC() == nullptr || SelectedCraftingItem.ItemID == 0) return;

	GetSCPC()->CraftItem(SelectedCraftingItem.ItemID, SelectedCraftingItem.ContainerType, SelectedCraftingItem.TableType);
}

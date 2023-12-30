// Copyright Fillipe Romero

#include "UI/WidgetController/SCCraftingMenuWidgetController.h"

#include "Enums/ContainerType.h"

void USCCraftingMenuWidgetController::CraftingItemSelected(const int32 ItemID, const EContainerType ContainerType)
{
	// Maybe I can just broadcast the item selected? I can have a reference of the table already.
	OnCraftingItemSelectedDelegate.Broadcast(ItemID, ContainerType);
}

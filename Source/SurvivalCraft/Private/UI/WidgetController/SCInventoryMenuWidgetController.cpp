﻿// Copyright Fillipe Romero

#include "UI/WidgetController/SCInventoryMenuWidgetController.h"
#include "Player/SCPlayerController.h"
#include "Player/SCPlayerState.h"

void USCInventoryMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnUpdateItemSlotDelegate.AddLambda(
		[this](EContainerType ContainerType, int32 SlotIndex, const FItemInformation& Item)
		{
			OnUpdateItemSlotWidgetDelegate.Broadcast(ContainerType, SlotIndex, Item);
		}
	);

	GetSCPC()->OnResetItemSlotDelegate.AddLambda(
		[this](EContainerType ContainerType, int32 SlotIndex)
		{
			OnResetItemSlotWidgetDelegate.Broadcast(ContainerType, SlotIndex);
		}
	);

	GetSCPC()->OnUpdateArmorSlotDelegate.AddLambda(
		[this](EArmorType ArmorType, const FItemInformation& Item)
		{
			OnUpdateArmorSlotWidgetDelegate.Broadcast(ArmorType, Item);
		}
	);

	GetSCPC()->OnResetArmorSlotDelegate.AddLambda(
		[this](EArmorType ArmorType)
		{
			OnResetArmorSlotWidgetDelegate.Broadcast(ArmorType);
		}
	);

	GetSCPC()->OnPlayerWindowInitDelegate.AddLambda(
		[this](UMaterialInstanceDynamic* Material)
		{
			OnPlayerWindowWidgetDelegate.Broadcast(Material);
		}
	);

	GetSCPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(static_cast<float>(NewLevel));	
		}
	);
}

// Copyright Fillipe Romero

#include "UI/WidgetController/SCHotbarMenuWidgetController.h"
#include "Player/SCPlayerController.h"

void USCHotbarMenuWidgetController::BindCallbacksToDependencies()
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
}

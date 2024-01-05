// Copyright Fillipe Romero

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

	GetSCPS()->OnLevelChangedDelegate.AddLambda(
	[this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);	
	}
);
}

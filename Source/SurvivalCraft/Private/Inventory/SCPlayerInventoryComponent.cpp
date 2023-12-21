// Copyright Fillipe Romero

#include "Inventory/SCPlayerInventoryComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Player/SCPlayerController.h"

USCPlayerInventoryComponent::USCPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USCPlayerInventoryComponent::AddItemToIndex(const FItemInformation& Item, int32 Index)
{
	const bool bIsSuccess = Super::AddItemToIndex(Item, Index);

	if (bIsSuccess)
	{
		ASCPlayerController* PC = IPlayerInterface::Execute_GetSCPlayerController(GetOwner());
		
		PC->UpdateItemSlot(ContainerType, Index, Item);
	}

	return bIsSuccess;
}

void USCPlayerInventoryComponent::HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex)
{
	Super::HandleSlotDrop(FromContainer, FromIndex, ToIndex);

	switch (FromContainer->ContainerType)
	{
	case EContainerType::ECT_PlayerInventory:
		FromContainer->TransferItem(this, FromIndex, ToIndex);
		break;
	case EContainerType::ECT_PlayerHotbar:
		break;
	case EContainerType::ECT_PlayerStorage:
		break;
	case EContainerType::ECT_PlayerArmor:
		break;
	}
}

bool USCPlayerInventoryComponent::RemoveItemByIndex(int32 Index)
{
	const bool bIsSuccess =  Super::RemoveItemByIndex(Index);
	
	if (bIsSuccess)
	{
		ASCPlayerController* PC = IPlayerInterface::Execute_GetSCPlayerController(GetOwner());
		
		PC->ResetItemSlot(ContainerType, Index);
	}

	return bIsSuccess;
}

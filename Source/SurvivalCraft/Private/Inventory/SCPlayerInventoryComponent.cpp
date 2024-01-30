// Copyright Fillipe Romero

#include "Inventory/SCPlayerInventoryComponent.h"
#include "Character/SCCharacter.h"
#include "Components/ArrowComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Items/SCWorldItemBag.h"
#include "Player/SCPlayerController.h"

USCPlayerInventoryComponent::USCPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool USCPlayerInventoryComponent::AddItemToIndex(const FItemInformation& Item, int32 FromIndex, int32 ToIndex, int32& OutRemainingQuantity)
{
	const bool bIsSuccess = Super::AddItemToIndex(Item, FromIndex, ToIndex, OutRemainingQuantity);

	if (bIsSuccess && OutRemainingQuantity == -1)
	{
		ASCPlayerController* PC = IPlayerInterface::Execute_GetSCPlayerController(GetOwner());
		
		PC->ClientUpdateItemSlot(ContainerType, ToIndex, Item);
	}

	return bIsSuccess;
}

void USCPlayerInventoryComponent::HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex)
{
	Super::HandleSlotDrop(FromContainer, FromIndex, ToIndex);

	switch (FromContainer->ContainerType)
	{
	case EContainerType::ECT_PlayerInventory:
	case EContainerType::ECT_PlayerHotbar:
	case EContainerType::ECT_PlayerStorage:
		FromContainer->TransferItem(this, FromIndex, ToIndex);
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
		
		PC->ClientResetItemSlot(ContainerType, Index);
	}

	return bIsSuccess;
}

void USCPlayerInventoryComponent::DropItem(int32 Index)
{
	Super::DropItem(Index);
	checkf(WorldItemBagClass, TEXT("WorldItemBagClass not set. Please fill out WorldItemBagClass property"));

	if (IsSlotEmpty(Index)) return;

	if (const ASCCharacter* Character = Cast<ASCCharacter>(GetOwner()))
	{
		FTransform Transform;
		Transform.SetLocation(Character->GetPlayerArrow()->GetComponentLocation());
		
		ASCWorldItemBag* SpawnedWorldItemBag = GetWorld()->SpawnActorDeferred<ASCWorldItemBag>(WorldItemBagClass, Transform);

		SpawnedWorldItemBag->SetBagItemInfo(GetItems()[Index]);
		SpawnedWorldItemBag->FinishSpawning(Transform);

		RemoveItemByIndex(Index);
	}
}

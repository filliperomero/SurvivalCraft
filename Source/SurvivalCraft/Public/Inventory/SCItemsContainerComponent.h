// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Crafting/Data/CraftingData.h"
#include "Enums/ContainerType.h"
#include "Items/Data/SCItemData.h"
#include "SCItemsContainerComponent.generated.h"

class ASCWorldItemBag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCItemsContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCItemsContainerComponent();

	virtual void InitializeItems(const FItemInformation& Item, const int32 Size);

	UFUNCTION(BlueprintCallable) // Provisory UFUNCTION
	virtual void AddItem(FItemInformation Item, bool bIsItemSplit = false);

	void OnSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex);
	void TransferItem(USCItemsContainerComponent* ToComponent, int32 FromIndex, int32 ToIndex);
	virtual bool AddItemToIndex(const FItemInformation& Item, int32 FromIndex, int32 ToIndex, int32& OutRemainingQuantity);
	bool StackItem(const FItemInformation& Item, int32 ToIndex, int32& OutRemainingQuantity);
	bool RemoveItems(TArray<FCraftingItemInfo> ItemsToRemove);
	bool ContainRequiredItems(TArray<FCraftingItemInfo> RequiredItems);
	bool RemoveItemQuantity(int32 Index, int32 AmountToRemove);
	bool UpdateItemQuantity(int32 Index, int32 NewQuantity);
	virtual bool RemoveItemByIndex(int32 Index);
	virtual bool IsEmpty();
	bool FindItem(int32 ItemID, int32& OutItemIndex, FItemInformation& OutItem);
	bool IsFull();
	/* This function will return a list of available items inside our Items Array (it removes the empty structs)*/
	TArray<FItemInformation> GetAvailableItems();
	// Override in child classes
	virtual void DropItem(int32 Index);
	virtual void SplitItemStack(int32 Index);

	TArray<FItemInformation> Items;

	UPROPERTY(EditDefaultsOnly, Category = "Items Container Properties")
	EContainerType ContainerType = EContainerType::ECT_PlayerInventory;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items Container Properties")
	TSubclassOf<ASCWorldItemBag> WorldItemBagClass;

	virtual void FindEmptySlot(bool& bSuccess, int32& OutEmptyIndex);
	FItemInformation GetItemByIndex(int32 Index);
	virtual bool IsSlotEmpty(int32 SlotIndex);
	virtual void UpdateUI(int32 Index, const FItemInformation& Item, bool bShouldResetSlot);
	bool HasItemsToStack(const FItemInformation& ItemToCheck);
	void AddRemainingItemQuantity(FItemInformation& Item);
	// Override in child classes
	virtual void HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex);

	UFUNCTION(Server, Reliable)
	void ServerAddItem(FItemInformation Item, bool bIsItemSplit);

public:
	TArray<FItemInformation> GetItems() const { return Items; }
};

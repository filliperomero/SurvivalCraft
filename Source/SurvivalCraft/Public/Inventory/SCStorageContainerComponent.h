// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemsContainerComponent.h"
#include "SCStorageContainerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCStorageContainerComponent : public USCItemsContainerComponent
{
	GENERATED_BODY()

public:
	USCStorageContainerComponent();
	
	virtual bool AddItemToIndex(const FItemInformation& Item, int32 FromIndex, int32 ToIndex, int32& OutRemainingQuantity) override;
	virtual bool RemoveItemByIndex(int32 Index) override;
	virtual void DropItem(int32 Index) override;

protected:
	virtual void HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex) override;
};

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemsContainerComponent.h"
#include "SCPlayerInventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCPlayerInventoryComponent : public USCItemsContainerComponent
{
	GENERATED_BODY()

public:
	USCPlayerInventoryComponent();

	virtual bool AddItemToIndex(const FItemInformation& Item, int32 Index) override;
	virtual bool RemoveItemByIndex(int32 Index) override;

protected:
	virtual void BeginPlay() override;
	virtual void HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex) override;
};

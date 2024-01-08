// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemsContainerComponent.h"
#include "SCPlayerHotbarComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCPlayerHotbarComponent : public USCItemsContainerComponent
{
	GENERATED_BODY()

public:
	USCPlayerHotbarComponent();

	bool HasItemInSlot(const int32 Index, EItemType& ItemType);
	virtual bool AddItemToIndex(const FItemInformation& Item, int32 Index) override;
	virtual bool RemoveItemByIndex(int32 Index) override;

protected:
	virtual void BeginPlay() override;

	virtual void HandleSlotDrop(USCItemsContainerComponent* FromContainer, int32 FromIndex, int32 ToIndex) override;
};

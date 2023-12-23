// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemMaster.h"
#include "Data/EquipableData.h"
#include "Interfaces/EquipableInterface.h"
#include "SCEquipableItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCEquipableItem : public ASCItemMaster, public IEquipableInterface
{
	GENERATED_BODY()

public:
	ASCEquipableItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipable Properties")
	FEquipableItemInfo EquipableItemInfo;

public:
	FORCEINLINE FEquipableItemInfo GetEquipableItemInfo() { return EquipableItemInfo; }
};

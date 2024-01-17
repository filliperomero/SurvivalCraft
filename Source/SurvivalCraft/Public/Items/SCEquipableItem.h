// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemMaster.h"
#include "BuildingSystem/Data/BuildableData.h"
#include "Data/EquipableData.h"
#include "Interfaces/EquipableInterface.h"
#include "Interfaces/StructureDamageInterface.h"
#include "SCEquipableItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCEquipableItem : public ASCItemMaster, public IEquipableInterface, public IStructureDamageInterface
{
	GENERATED_BODY()

public:
	ASCEquipableItem();

	/** StructureDamageInterface */
	virtual FDamageTiers GetDamageTiers_Implementation() override;
	/** StructureDamageInterface */

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipable Properties")
	FEquipableItemInfo EquipableItemInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Equipable Properties")
	FDamageTiers StructureDamageTier;

public:
	FORCEINLINE FEquipableItemInfo GetEquipableItemInfo() { return EquipableItemInfo; }
};

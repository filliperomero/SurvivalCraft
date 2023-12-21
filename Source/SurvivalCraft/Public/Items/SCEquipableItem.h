// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemMaster.h"
#include "SCEquipableItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCEquipableItem : public ASCItemMaster
{
	GENERATED_BODY()

public:
	ASCEquipableItem();

protected:
	virtual void BeginPlay() override;
};

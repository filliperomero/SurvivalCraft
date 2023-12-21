// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCFirstPersonEquipable.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCFirstPersonEquipable : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCFirstPersonEquipable();

protected:
	virtual void BeginPlay() override;
};

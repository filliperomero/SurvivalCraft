// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCThirdPersonEquipable.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCThirdPersonEquipable : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCThirdPersonEquipable();

protected:
	virtual void BeginPlay() override;
};

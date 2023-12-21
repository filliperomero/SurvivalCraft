// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/Equipables/SCFirstPersonEquipable.h"
#include "SCHatchet.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCHatchet : public ASCFirstPersonEquipable
{
	GENERATED_BODY()

public:
	ASCHatchet();

protected:
	virtual void BeginPlay() override;
};

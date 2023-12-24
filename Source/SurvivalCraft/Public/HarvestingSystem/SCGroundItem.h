// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCHarvesting.h"
#include "SCGroundItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCGroundItem : public ASCHarvesting
{
	GENERATED_BODY()

public:
	ASCGroundItem();

protected:
	virtual void BeginPlay() override;
};

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/Data/BuildableData.h"
#include "UObject/Interface.h"
#include "StructureDamageInterface.generated.h"

UINTERFACE(MinimalAPI)
class UStructureDamageInterface : public UInterface
{
	GENERATED_BODY()
};

class SURVIVALCRAFT_API IStructureDamageInterface
{
	GENERATED_BODY()

public:
	/** Function responsible to get the Damage Tiers to check if we can damage the structure or not */
	UFUNCTION(BlueprintNativeEvent)
	FDamageTiers GetDamageTiers();
};

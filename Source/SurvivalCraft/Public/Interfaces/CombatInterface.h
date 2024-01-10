// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class SURVIVALCRAFT_API ICombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetArmorAmount();
};

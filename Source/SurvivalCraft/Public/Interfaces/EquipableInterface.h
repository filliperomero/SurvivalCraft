// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipableInterface.generated.h"

class ASCCharacter;

UINTERFACE(MinimalAPI)
class UEquipableInterface : public UInterface
{
	GENERATED_BODY()
};

class SURVIVALCRAFT_API IEquipableInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UseItem(ASCCharacter* SCCharacter);
};

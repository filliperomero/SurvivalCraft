// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemsContainerComponent.h"
#include "SCPlayerInventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCPlayerInventoryComponent : public USCItemsContainerComponent
{
	GENERATED_BODY()

public:
	USCPlayerInventoryComponent();

protected:
	virtual void BeginPlay() override;
};

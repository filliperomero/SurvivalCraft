// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemsContainerComponent.h"
#include "SCStorageContainerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCStorageContainerComponent : public USCItemsContainerComponent
{
	GENERATED_BODY()

public:
	USCStorageContainerComponent();

protected:
	virtual void BeginPlay() override;
};

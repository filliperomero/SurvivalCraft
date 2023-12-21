// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCItemsContainerComponent.h"
#include "SCPlayerHotbarComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCPlayerHotbarComponent : public USCItemsContainerComponent
{
	GENERATED_BODY()

public:
	USCPlayerHotbarComponent();

protected:
	virtual void BeginPlay() override;
};

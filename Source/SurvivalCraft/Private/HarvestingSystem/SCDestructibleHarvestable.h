// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCDestructibleHarvestable.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCDestructibleHarvestable : public AActor
{
	GENERATED_BODY()

public:
	ASCDestructibleHarvestable();

protected:
	virtual void BeginPlay() override;
};

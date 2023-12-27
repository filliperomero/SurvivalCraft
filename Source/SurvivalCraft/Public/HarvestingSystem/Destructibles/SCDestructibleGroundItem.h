// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "SCDestructibleGroundItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCDestructibleGroundItem : public ASCDestructibleHarvestable
{
	GENERATED_BODY()

public:
	ASCDestructibleGroundItem();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;
};

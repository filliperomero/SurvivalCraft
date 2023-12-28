// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "SCDestructibleRock.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCDestructibleRock : public ASCDestructibleHarvestable
{
	GENERATED_BODY()

public:
	ASCDestructibleRock();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGeometryCollectionComponent> GeometryCollectionComponent;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UParticleSystem> DestroyEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<USoundBase> DestroySound;
};

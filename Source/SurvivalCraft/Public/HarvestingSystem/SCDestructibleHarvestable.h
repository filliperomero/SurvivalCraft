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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DestroyCooldown = 15.f;

	UPROPERTY(Replicated)
	FVector DirectionToFall = FVector::ZeroVector;

public:
	FORCEINLINE void SetDirectionToFall(const FVector& InDirection) { DirectionToFall = InDirection; }
};

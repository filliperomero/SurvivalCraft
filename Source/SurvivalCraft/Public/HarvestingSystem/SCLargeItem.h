// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCHarvesting.h"
#include "SCLargeItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCLargeItem : public ASCHarvesting
{
	GENERATED_BODY()

public:
	ASCLargeItem();

	void ReceiveDamage(float InDamage);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float Health = 50.f;

public:
	FORCEINLINE float GetHealth() const { return Health; }
};

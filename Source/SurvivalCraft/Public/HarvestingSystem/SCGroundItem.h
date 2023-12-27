// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCHarvesting.h"
#include "SCGroundItem.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCGroundItem : public ASCHarvesting
{
	GENERATED_BODY()

public:
	ASCGroundItem();
	void ReceiveDamage(float InDamage);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Properties")
	float Health = 50.f;

public:
	FORCEINLINE float GetHealth() const { return Health; }
};

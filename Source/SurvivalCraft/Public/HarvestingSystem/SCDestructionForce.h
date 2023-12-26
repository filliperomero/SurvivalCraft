// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "SCDestructionForce.generated.h"

class USphereComponent;
class URadialFalloff;

UCLASS()
class SURVIVALCRAFT_API ASCDestructionForce : public AFieldSystemActor
{
	GENERATED_BODY()

public:
	ASCDestructionForce();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URadialFalloff> RadialFalloff;
};

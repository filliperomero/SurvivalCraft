// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCBuildable.h"
#include "SCFoundation.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCFoundation : public ASCBuildable
{
	GENERATED_BODY()

public:
	ASCFoundation();

protected:
	virtual void BeginPlay() override;
};

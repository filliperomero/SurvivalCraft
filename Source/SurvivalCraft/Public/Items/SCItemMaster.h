// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCItemMaster.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCItemMaster : public AActor
{
	GENERATED_BODY()

public:
	ASCItemMaster();

protected:
	virtual void BeginPlay() override;
};

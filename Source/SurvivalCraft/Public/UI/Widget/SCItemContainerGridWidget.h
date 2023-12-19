// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCUserWidget.h"
#include "Enums/ContainerType.h"
#include "SCItemContainerGridWidget.generated.h"

UCLASS()
class SURVIVALCRAFT_API USCItemContainerGridWidget : public USCUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Properties")
	int32 SlotsPerRow = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Properties")
	int32 TotalSlots = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid Properties")
	EContainerType ContainerType = EContainerType::ECT_PlayerInventory;
};

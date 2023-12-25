// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/Equipables/SCFirstPersonEquipable.h"
#include "SCHatchet.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCHatchet : public ASCFirstPersonEquipable
{
	GENERATED_BODY()

public:
	ASCHatchet();

	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter) override;
	virtual void Interact_Implementation(const FVector& LocationToCheck) override;
	/** EquipableInterface */
	
	void HarvestFoliage(float Damage, AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	TObjectPtr<UDataTable> LargeItemsResourceDataTable;
};

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCPickaxe.generated.h"

struct FResourceInfo;

UCLASS()
class SURVIVALCRAFT_API ASCPickaxe : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCPickaxe();

	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter) override;
	virtual void Interact_Implementation(const FVector& LocationToCheck) override;
	/** EquipableInterface */
	
	void HarvestFoliage(float Damage, AActor* Target);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources")
	TObjectPtr<UDataTable> LargeItemsResourceDataTable;

private:
	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	float ToolDamage = 15.f;
	
	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	EToolTier ToolTier = EToolTier::ETT_Stone;

	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	EHarvestingToolType ToolType = EHarvestingToolType::EHTT_Pickaxe;

	int32 CalculateGivenQuantity(const FResourceInfo& Resource) const;
};

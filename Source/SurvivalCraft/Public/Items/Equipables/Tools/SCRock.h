// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCRock.generated.h"

struct FResourceInfo;

UCLASS()
class SURVIVALCRAFT_API ASCRock : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCRock();

	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation) override;
	virtual void Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation) override;
	/** EquipableInterface */

private:
	UPROPERTY(EditAnywhere, Category = "Resources")
	TObjectPtr<UDataTable> LargeItemsResourceDataTable;
	
	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	float TraceDistance = 100.f;

	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	TObjectPtr<UParticleSystem> HitEffect;

	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	TObjectPtr<USoundBase> HitSound;
	
	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	EToolTier ToolTier = EToolTier::ETT_Stone;

	UPROPERTY(EditAnywhere, Category = "Tool Properties")
	EHarvestingToolType ToolType = EHarvestingToolType::EHTT_Rock;
	
	void HarvestFoliage(float Damage, AActor* Target);
	
	int32 CalculateGivenQuantity(const FResourceInfo& Resource) const;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastHitEffect(const FVector_NetQuantize& Location, const FRotator& Rotation);
};

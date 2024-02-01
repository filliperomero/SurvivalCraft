// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCRifle.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCRifle : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCRifle();

	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation) override;
	/** EquipableInterface */

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> RifleSkeletal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BulletDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DamageMultiplier = 0.05f;

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWeaponAnim();
};

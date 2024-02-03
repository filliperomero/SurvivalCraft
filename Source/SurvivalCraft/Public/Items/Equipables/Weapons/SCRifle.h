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
	virtual void ReloadItem_Implementation(ASCCharacter* SCCharacter) override;
	virtual void Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation) override;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AmmoItemID = FName();

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWeaponAnim();

	void SpendRound(ASCCharacter* SCCharacter);
};

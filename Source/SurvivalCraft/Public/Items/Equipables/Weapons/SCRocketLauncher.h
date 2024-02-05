// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCRocketLauncher.generated.h"

class UArrowComponent;
class ASCRocketProjectile;

UCLASS()
class SURVIVALCRAFT_API ASCRocketLauncher : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCRocketLauncher();

	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation) override;
	virtual void ReloadItem_Implementation(ASCCharacter* SCCharacter) override;
	virtual void Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation) override;
	/** EquipableInterface */

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimationAsset> FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASCRocketProjectile> RocketProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AmmoItemID = FName();

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWeaponAnim();

	void SpendRound(ASCCharacter* SCCharacter);
};

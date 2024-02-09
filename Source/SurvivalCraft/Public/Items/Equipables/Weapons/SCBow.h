// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCBow.generated.h"

class ASCArrowProjectile;

UCLASS()
class SURVIVALCRAFT_API ASCBow : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCBow();
	void PlayFireAnimation(bool bReset);
	
	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation) override;
	virtual void LeftButtonReleased_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation) override;
	virtual void ReloadItem_Implementation(ASCCharacter* SCCharacter) override;
	// virtual void Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation) override;
	/** EquipableInterface */

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> ArrowStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimationAsset> FireAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASCArrowProjectile> ArrowProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AmmoItemID = FName();

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayWeaponAnim(ASCCharacter* SCCharacter);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastResetWeaponAnim(ASCCharacter* SCCharacter);

	void SpendRound(ASCCharacter* SCCharacter);
};

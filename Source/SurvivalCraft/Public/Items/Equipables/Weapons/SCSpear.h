// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCEquipableItem.h"
#include "SCSpear.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCSpear : public ASCEquipableItem
{
	GENERATED_BODY()

public:
	ASCSpear();

	/** EquipableInterface */
	virtual void UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation) override;
	virtual void Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation) override;
	/** EquipableInterface */
};

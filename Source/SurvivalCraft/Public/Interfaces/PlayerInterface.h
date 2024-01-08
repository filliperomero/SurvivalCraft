// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Enums/ContainerType.h"
#include "Items/Data/SCItemData.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class ASCPlayerController;

class SURVIVALCRAFT_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	ASCPlayerController* GetSCPlayerController();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlotDrop(EContainerType TargetContainerType, EContainerType FromContainerType, int32 FromIndex, int32 ToIndex, EArmorType ArmorType);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnEquipArmor(EContainerType FromContainerType, int32 FromIndex, EArmorType ArmorType);

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void SpendSkillPoint(EPlayerStats StatToUpgrade);
};

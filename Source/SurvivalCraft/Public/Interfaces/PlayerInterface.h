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
	void OnSlotDrop(EContainerType TargetContainer, EContainerType FromContainer, int32 FromIndex, int32 ToIndex, EArmorType ArmorType);
};

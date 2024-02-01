// Copyright Fillipe Romero

#include "Items/SCEquipableItem.h"

ASCEquipableItem::ASCEquipableItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

FDamageTiers ASCEquipableItem::GetDamageTiers_Implementation()
{
	return StructureDamageTier;
}

float ASCEquipableItem::GetEquipableDelay_Implementation()
{
	return UseDelay;
}

void ASCEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

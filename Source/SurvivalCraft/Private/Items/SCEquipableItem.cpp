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

void ASCEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

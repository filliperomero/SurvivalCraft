// Copyright Fillipe Romero

#include "Items/Equipables/Tools/SCHatchet.h"
#include "Character/SCCharacter.h"

ASCHatchet::ASCHatchet()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASCHatchet::UseItem_Implementation(ASCCharacter* SCCharacter)
{
	// TODO: Better to use a Interface here, so we don't need to import SCCharacter
	SCCharacter->PlayEquipableMontage(FName("Hatchet"));
}

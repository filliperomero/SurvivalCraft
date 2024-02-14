// Copyright Fillipe Romero

#include "Game/SCGameState.h"

void ASCGameState::CreateTribe(const FTribeInfo& Tribe)
{
	// Already exist a tribe with this ID
	if (TribeMap.Find(Tribe.ID) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("A tribe with this ID (%s) already exist."), *Tribe.ID)
		return;
	}
	
	TribeMap.Add(Tribe.ID, Tribe);
}

// Copyright Fillipe Romero

#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "Net/UnrealNetwork.h"

ASCDestructibleHarvestable::ASCDestructibleHarvestable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
}

void ASCDestructibleHarvestable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCDestructibleHarvestable, DirectionToFall);
}

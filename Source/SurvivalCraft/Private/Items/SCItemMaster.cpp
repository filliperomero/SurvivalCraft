// Copyright Fillipe Romero

#include "Items/SCItemMaster.h"

ASCItemMaster::ASCItemMaster()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);
	BaseMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BaseMesh->SetCastShadow(false);
}

void ASCItemMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

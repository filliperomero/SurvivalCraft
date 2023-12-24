// Copyright Fillipe Romero

#include "HarvestingSystem/SCLargeItem.h"

ASCLargeItem::ASCLargeItem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetMobility(EComponentMobility::Static);
}

void ASCLargeItem::BeginPlay()
{
	Super::BeginPlay();
}

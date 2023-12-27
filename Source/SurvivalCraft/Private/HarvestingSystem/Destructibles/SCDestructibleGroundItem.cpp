// Copyright Fillipe Romero

#include "HarvestingSystem/Destructibles/SCDestructibleGroundItem.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "HarvestingSystem/SCDestructionForce.h"

ASCDestructibleGroundItem::ASCDestructibleGroundItem()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASCDestructibleGroundItem::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SpawnActor<ASCDestructionForce>(ASCDestructionForce::StaticClass(), GetActorTransform());
	SetLifeSpan(DestroyCooldown);
}

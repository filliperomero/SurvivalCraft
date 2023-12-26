// Copyright Fillipe Romero

#include "HarvestingSystem/Destructibles/SCDestructibleRock.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "HarvestingSystem/SCDestructionForce.h"

ASCDestructibleRock::ASCDestructibleRock()
{
	PrimaryActorTick.bCanEverTick = false;

	GeometryCollectionComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollectionComponent"));
	SetRootComponent(GeometryCollectionComponent);
	GeometryCollectionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASCDestructibleRock::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->SpawnActor<ASCDestructionForce>(ASCDestructionForce::StaticClass(), GetActorTransform());
	SetLifeSpan(DestroyCooldown);
}

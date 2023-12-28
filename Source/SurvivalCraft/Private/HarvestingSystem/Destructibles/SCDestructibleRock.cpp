// Copyright Fillipe Romero

#include "HarvestingSystem/Destructibles/SCDestructibleRock.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "HarvestingSystem/SCDestructionForce.h"
#include "Kismet/GameplayStatics.h"

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
	
	if (DestroyEffect)
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyEffect, GetActorLocation());

	if (DestroySound)
		UGameplayStatics::PlaySoundAtLocation(this, DestroySound, GetActorLocation());
	
	SetLifeSpan(DestroyCooldown);
}

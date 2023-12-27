// Copyright Fillipe Romero

#include "HarvestingSystem/SCGroundItem.h"

ASCGroundItem::ASCGroundItem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionObjectType(ECC_WorldStatic);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ASCGroundItem::ReceiveDamage(float InDamage)
{
	Health = FMath::Clamp(Health - InDamage, 0, 100.f);
}

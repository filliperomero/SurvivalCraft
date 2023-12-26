// Copyright Fillipe Romero

#include "HarvestingSystem/SCLargeItem.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCLargeItem::ASCLargeItem()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Ignore);
	MeshComponent->SetCollisionObjectType(ECC_STRUCTURE);
}

void ASCLargeItem::ReceiveDamage(float InDamage)
{
	// TODO: Create a maxHealth variable
	Health = FMath::Clamp(Health - InDamage, 0, 100.f);
}

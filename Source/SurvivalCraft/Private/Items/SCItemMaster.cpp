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

// TODO: Check the necessity of setting the ItemInfo, maybe we can set it when spawning the item
void ASCItemMaster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemsDataTable == nullptr || ItemRowName.IsNone()) return;

	if (const FItemInformation* ItemInformation = ItemsDataTable->FindRow<FItemInformation>(ItemRowName, TEXT("")))
	{
		ItemInfo = *ItemInformation;
	}
}

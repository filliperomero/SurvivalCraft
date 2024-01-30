// Copyright Fillipe Romero

#include "Items/SCWorldItemBag.h"
#include "Character/SCCharacter.h"
#include "Inventory/SCPlayerInventoryComponent.h"

ASCWorldItemBag::ASCWorldItemBag()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic); // In the course, it is WorldStatic
	Mesh->SetMassOverrideInKg(NAME_None, 100.f);
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ASCWorldItemBag::InteractEvent_Implementation(ASCCharacter* Character)
{
	Character->GetPlayerInventoryComponent()->AddItem(BagItemInfo);
	Character->ClientShowItemAdded(BagItemInfo.ItemIcon, BagItemInfo.ItemQuantity, BagItemInfo.ItemName);

	Destroy();
}

void ASCWorldItemBag::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(DespawnBagTimer, this, &ThisClass::DespawnBagFinished, DespawnBagDelay);
	}
}

void ASCWorldItemBag::DespawnBagFinished()
{
	Destroy();
}

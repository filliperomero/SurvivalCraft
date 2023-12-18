// Copyright Fillipe Romero

#include "Inventory/SCPlayerInventoryComponent.h"

USCPlayerInventoryComponent::USCPlayerInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCPlayerInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

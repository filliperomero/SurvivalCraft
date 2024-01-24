// Copyright Fillipe Romero

#include "Inventory/SCStorageContainerComponent.h"

USCStorageContainerComponent::USCStorageContainerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCStorageContainerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Copyright Fillipe Romero

#include "Inventory/SCPlayerHotbarComponent.h"

USCPlayerHotbarComponent::USCPlayerHotbarComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USCPlayerHotbarComponent::BeginPlay()
{
	Super::BeginPlay();
}

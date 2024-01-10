// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildable.h"

ASCBuildable::ASCBuildable()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ASCBuildable::BeginPlay()
{
	Super::BeginPlay();
}

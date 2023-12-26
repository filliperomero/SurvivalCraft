// Copyright Fillipe Romero

#include "HarvestingSystem/SCDestructionForce.h"
#include "Components/SphereComponent.h"
#include "Field/FieldSystemComponent.h"

ASCDestructionForce::ASCDestructionForce()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSphereRadius(32.f);

	RadialFalloff = CreateDefaultSubobject<URadialFalloff>(TEXT("RadialFalloff"));
}

void ASCDestructionForce::BeginPlay()
{
	Super::BeginPlay();

	RadialFalloff->SetRadialFalloff(500'000.f, 0.f, 1.f, 0.f, SphereComponent->GetScaledSphereRadius(), SphereComponent->GetComponentLocation(), Field_FallOff_None);

	GetFieldSystemComponent()->AddFieldCommand(true, Field_ExternalClusterStrain, nullptr, RadialFalloff);

	SetLifeSpan(2.f);
}

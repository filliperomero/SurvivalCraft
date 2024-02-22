// Copyright Fillipe Romero

#include "BuildingSystem/SCTorch.h"

#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

ASCTorch::ASCTorch()
{
	PrimaryActorTick.bCanEverTick = false;

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(GetMesh());
	FireEffect->SetRelativeLocation(FVector(-30.f, -50.f, 50.f));
	FireEffect->SetAutoActivate(false);
}

void ASCTorch::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCTorch, bIsTorchLit);
}

void ASCTorch::DestroyStructure(const bool bLog)
{
	bIsTorchLit = false;
	ToggleTorch();
	
	Super::DestroyStructure(bLog);
}

void ASCTorch::InteractEvent_Implementation(ASCCharacter* Character)
{
	bIsTorchLit = !bIsTorchLit;

	ToggleTorch();
}

void ASCTorch::ToggleTorch()
{
	if (bIsTorchLit)
	{
		FireEffect->Activate();
	}
	else
	{
		FireEffect->Deactivate();
	}
}

void ASCTorch::OnRep_IsTorchLit()
{
	ToggleTorch();
}

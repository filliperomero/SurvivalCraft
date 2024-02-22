// Copyright Fillipe Romero

#include "BuildingSystem/SCRoof.h"
#include "Components/BoxComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCRoof::ASCRoof()
{
	PrimaryActorTick.bCanEverTick = false;

	RoofBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RoofBox"));
	RoofBox->SetupAttachment(GetMesh());
	RoofBox->SetRelativeLocation(FVector(0.f, -143.f, 0.f));
	RoofBox->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	RoofBox->SetBoxExtent(FVector(150.f, 150.f, 150.f));
	RoofBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	RoofBox->SetCollisionResponseToChannel(ECC_ROOF_TRACE, ECR_Block);
	RoofBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	Tags.Add(FName("AboveFoundation"));
}

void ASCRoof::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(RoofBox);

	SetSnapBoxes(BoxesToSnap);
}

void ASCRoof::DestroyStructure(const bool bLog)
{
	Super::DestroyStructure(bLog);

	TArray<AActor*> OverlappingActors;
	// TODO: Change this for ASCRoofLarge class when it is created
	RoofBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
	if (OverlappingActors.Num() > 0)
	{
		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (ASCBuildable* Buildable = Cast<ASCBuildable>(OverlappingActor))
			{
				Buildable->DestroyStructure(bLog);
			}
		}
	}
}

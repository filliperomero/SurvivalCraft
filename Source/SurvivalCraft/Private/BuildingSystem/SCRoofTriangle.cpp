// Copyright Fillipe Romero

#include "BuildingSystem/SCRoofTriangle.h"
#include "Components/BoxComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCRoofTriangle::ASCRoofTriangle()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	RoofBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RoofBox"));
	RoofBox->SetupAttachment(GetMesh());
	RoofBox->SetRelativeLocation(FVector(99.f, -143.f, 0.f));
	RoofBox->SetBoxExtent(FVector(130.f, 150.f, 50.f));
	RoofBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	RoofBox->SetCollisionResponseToChannel(ECC_ROOF_SMALL_TRACE, ECR_Block);

	RoofBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("RoofBox2"));
	RoofBox2->SetupAttachment(GetMesh());
	RoofBox2->SetRelativeLocation(FVector(-99.f, -143.f, 0.f));
	RoofBox2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	RoofBox2->SetBoxExtent(FVector(130.f, 150.f, 50.f));
	RoofBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	RoofBox2->SetCollisionResponseToChannel(ECC_ROOF_SMALL_TRACE, ECR_Block);

	Tags.Add(FName("AboveFoundation"));
}

void ASCRoofTriangle::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(RoofBox);
	BoxesToSnap.Add(RoofBox2);

	SetSnapBoxes(BoxesToSnap);
}

void ASCRoofTriangle::DestroyStructure()
{
	Super::DestroyStructure();

	TArray<AActor*> OverlappingActors;
	OverlapBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
	if (OverlappingActors.Num() > 0)
	{
		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (ASCBuildable* Buildable = Cast<ASCBuildable>(OverlappingActor))
			{
				Buildable->DestroyStructure();
			}
		}
	}
}

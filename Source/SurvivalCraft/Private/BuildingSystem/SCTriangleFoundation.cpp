// Copyright Fillipe Romero

#include "BuildingSystem/SCTriangleFoundation.h"

#include "Components/BoxComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCTriangleFoundation::ASCTriangleFoundation()
{
	PrimaryActorTick.bCanEverTick = false;

	FoundationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox"));
	FoundationBox->SetupAttachment(GetMesh());
	FoundationBox->SetRelativeLocation(FVector(0.f, 237.f, 0.f));
	FoundationBox->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	FoundationBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);
	FoundationBox->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);
	
	FoundationBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox2"));
	FoundationBox2->SetupAttachment(GetMesh());
	FoundationBox2->SetRelativeLocation(FVector(205.f, -118.5f, 0.f));
	FoundationBox2->SetRelativeRotation(FRotator(0.f, 60.f, 0.f));
	FoundationBox2->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	FoundationBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox2->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);
	
	FoundationBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox3"));
	FoundationBox3->SetupAttachment(GetMesh());
	FoundationBox3->SetRelativeLocation(FVector(-205.f, -118.5f, 0.f));
	FoundationBox3->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	FoundationBox3->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	FoundationBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox3->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);

	TriangleFoundationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox"));
	TriangleFoundationBox->SetupAttachment(GetMesh());
	TriangleFoundationBox->SetRelativeLocation(FVector(0.f, 173.f, 0.f));
	TriangleFoundationBox->SetRelativeRotation(FRotator(0.f, 60.f, 0.f));
	TriangleFoundationBox->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	TriangleFoundationBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);
	
	TriangleFoundationBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox2"));
	TriangleFoundationBox2->SetupAttachment(GetMesh());
	TriangleFoundationBox2->SetRelativeLocation(FVector(149.7f, -87.f, 0.f));
	TriangleFoundationBox2->SetRelativeRotation(FRotator(0.f, 60.f, 0.f));
	TriangleFoundationBox2->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	TriangleFoundationBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox2->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);
	
	TriangleFoundationBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox3"));
	TriangleFoundationBox3->SetupAttachment(GetMesh());
	TriangleFoundationBox3->SetRelativeLocation(FVector(-149.f, -87.f, 0.f));
	TriangleFoundationBox3->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	TriangleFoundationBox3->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	TriangleFoundationBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox3->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);

	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	WallBox->SetupAttachment(GetMesh());
	WallBox->SetRelativeLocation(FVector(0.f, 89.f, 235.f));
	WallBox->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox2"));
	WallBox2->SetupAttachment(GetMesh());
	WallBox2->SetRelativeLocation(FVector(77.f, -45.f, 235.f));
	WallBox2->SetRelativeRotation(FRotator(0.f, -120.f, 0.f));
	WallBox2->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox2->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox3"));
	WallBox3->SetupAttachment(GetMesh());
	WallBox3->SetRelativeLocation(FVector(-77.f, -45.f, 235.f));
	WallBox3->SetRelativeRotation(FRotator(0.f, -240.f, 0.f));
	WallBox3->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox3->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	RampBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RampBox"));
	RampBox->SetupAttachment(GetMesh());
	RampBox->SetRelativeLocation(FVector(-205.f, -118.3f, 0.f));
	RampBox->SetRelativeRotation(FRotator(0.f, 120.f, 0.f));
	RampBox->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	RampBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	RampBox->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);
	
	RampBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("RampBox2"));
	RampBox2->SetupAttachment(GetMesh());
	RampBox2->SetRelativeLocation(FVector(205.f, -118.3f, 0.f));
	RampBox2->SetRelativeRotation(FRotator(0.f, -120.f, 0.f));
	RampBox2->SetBoxExtent(FVector(150.f, 150.f, 85.f));
	RampBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	RampBox2->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);

	CheckStructureTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckStructureTraceBox"));
	CheckStructureTraceBox->SetupAttachment(RootComponent);
	CheckStructureTraceBox->SetRelativeLocation(FVector(0.f, 14.f, 160.f));
	CheckStructureTraceBox->SetBoxExtent(FVector(64.f, 72.f, 82.f));
	CheckStructureTraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckStructureTraceBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);
}

void ASCTriangleFoundation::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(FoundationBox);
	BoxesToSnap.Add(FoundationBox2);
	BoxesToSnap.Add(FoundationBox3);
	BoxesToSnap.Add(WallBox);
	BoxesToSnap.Add(WallBox2);
	BoxesToSnap.Add(WallBox3);
	BoxesToSnap.Add(TriangleFoundationBox);
	BoxesToSnap.Add(TriangleFoundationBox2);
	BoxesToSnap.Add(TriangleFoundationBox3);
	BoxesToSnap.Add(RampBox);
	BoxesToSnap.Add(RampBox2);

	SetSnapBoxes(BoxesToSnap);
}

void ASCTriangleFoundation::DestroyStructure(const bool bLog)
{
	Super::DestroyStructure(bLog);

	TArray<AActor*> OverlappingActors;
	CheckStructureTraceBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
	if (OverlappingActors.Num() <= 0) return;

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag(FName("AboveFoundation")))
		{
			if (ASCBuildable* Buildable = Cast<ASCBuildable>(OverlappingActor))
			{
				Buildable->DestroyStructure(bLog);
			}
		}
	}
}

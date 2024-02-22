// Copyright Fillipe Romero

#include "BuildingSystem/SCTriangleCeiling.h"

#include "Components/BoxComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCTriangleCeiling::ASCTriangleCeiling()
{
	PrimaryActorTick.bCanEverTick = false;

	CeilingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox"));
	CeilingBox->SetupAttachment(GetMesh());
	CeilingBox->SetRelativeLocation(FVector(0.f, 235.f, 0.f));
	CeilingBox->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	CeilingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	CeilingBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox2"));
	CeilingBox2->SetupAttachment(GetMesh());
	CeilingBox2->SetRelativeLocation(FVector(205.f, -118.f, 0.f));
	CeilingBox2->SetRelativeRotation(FRotator(0.f, -30.f, 0.f));
	CeilingBox2->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	CeilingBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox2->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	CeilingBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox3"));
	CeilingBox3->SetupAttachment(GetMesh());
	CeilingBox3->SetRelativeLocation(FVector(-205.f, -118.f, 0.f));
	CeilingBox3->SetRelativeRotation(FRotator(0.f, 30.f, 0.f));
	CeilingBox3->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	CeilingBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox3->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	TriangleCeilingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox"));
	TriangleCeilingBox->SetupAttachment(GetMesh());
	TriangleCeilingBox->SetRelativeLocation(FVector(0.f, 173.5f, 0.f));
	TriangleCeilingBox->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	TriangleCeilingBox->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	TriangleCeilingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);

	TriangleCeilingBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox2"));
	TriangleCeilingBox2->SetupAttachment(GetMesh());
	TriangleCeilingBox2->SetRelativeLocation(FVector(150.f, -86.5f, 0.f));
	TriangleCeilingBox2->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	TriangleCeilingBox2->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	TriangleCeilingBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox2->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);
	
	TriangleCeilingBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox3"));
	TriangleCeilingBox3->SetupAttachment(GetMesh());
	TriangleCeilingBox3->SetRelativeLocation(FVector(-150.f, -86.5f, 0.f));
	TriangleCeilingBox3->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	TriangleCeilingBox3->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	TriangleCeilingBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox3->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);

	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	WallBox->SetupAttachment(GetMesh());
	WallBox->SetRelativeLocation(FVector(0.f, 89.f, 158.f));
	WallBox->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox2"));
	WallBox2->SetupAttachment(GetMesh());
	WallBox2->SetRelativeLocation(FVector(77.f, -43.f, 158.f));
	WallBox2->SetRelativeRotation(FRotator(0.f, -120.f, 0.f));
	WallBox2->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox2->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox3"));
	WallBox3->SetupAttachment(GetMesh());
	WallBox3->SetRelativeLocation(FVector(-77.f, -43.f, 158.f));
	WallBox3->SetRelativeRotation(FRotator(0.f, 120.f, 0.f));
	WallBox3->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox3->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);

	CheckStructureTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckStructureTraceBox"));
	CheckStructureTraceBox->SetupAttachment(RootComponent);
	CheckStructureTraceBox->SetRelativeLocation(FVector(0.f, 14.f, 82.f));
	CheckStructureTraceBox->SetBoxExtent(FVector(64.f, 72.f, 82.f));
	CheckStructureTraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckStructureTraceBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	Tags.Add(FName("AboveWall"));
}

void ASCTriangleCeiling::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(CeilingBox);
	BoxesToSnap.Add(CeilingBox2);
	BoxesToSnap.Add(CeilingBox3);
	BoxesToSnap.Add(WallBox);
	BoxesToSnap.Add(WallBox2);
	BoxesToSnap.Add(WallBox3);
	BoxesToSnap.Add(TriangleCeilingBox);
	BoxesToSnap.Add(TriangleCeilingBox2);
	BoxesToSnap.Add(TriangleCeilingBox3);

	SetSnapBoxes(BoxesToSnap);
}

void ASCTriangleCeiling::DestroyStructure(const bool bLog)
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

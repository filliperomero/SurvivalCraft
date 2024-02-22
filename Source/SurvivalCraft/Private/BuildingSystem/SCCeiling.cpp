// Copyright Fillipe Romero

#include "BuildingSystem/SCCeiling.h"
#include "Components/BoxComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCCeiling::ASCCeiling()
{
	PrimaryActorTick.bCanEverTick = false;

	CeilingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox"));
	CeilingBox->SetupAttachment(GetMesh());
	CeilingBox->SetRelativeLocation(FVector(0.f, 300.f, 0.f));
	CeilingBox->SetBoxExtent(FVector(110.f, 110.f, 15.f));
	CeilingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	CeilingBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox2"));
	CeilingBox2->SetupAttachment(GetMesh());
	CeilingBox2->SetRelativeLocation(FVector(0.f, -300.f, 0.f));
	CeilingBox2->SetBoxExtent(FVector(110.f, 110.f, 15.f));
	CeilingBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox2->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	CeilingBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox3"));
	CeilingBox3->SetupAttachment(GetMesh());
	CeilingBox3->SetRelativeLocation(FVector(300.f, 0.f, 0.f));
	CeilingBox3->SetBoxExtent(FVector(110.f, 110.f, 15.f));
	CeilingBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox3->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	CeilingBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox4"));
	CeilingBox4->SetupAttachment(GetMesh());
	CeilingBox4->SetRelativeLocation(FVector(-300.f, 0.f, 0.f));
	CeilingBox4->SetBoxExtent(FVector(110.f, 110.f, 15.f));
	CeilingBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox4->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	TriangleCeilingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox"));
	TriangleCeilingBox->SetupAttachment(GetMesh());
	TriangleCeilingBox->SetRelativeLocation(FVector(0.f, 236.f, 0.f));
	TriangleCeilingBox->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	TriangleCeilingBox->SetBoxExtent(FVector(100.f, 100.f, 10.f));
	TriangleCeilingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);

	TriangleCeilingBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox2"));
	TriangleCeilingBox2->SetupAttachment(GetMesh());
	TriangleCeilingBox2->SetRelativeLocation(FVector(236.f, 0.f, 0.f));
	TriangleCeilingBox2->SetRelativeRotation(FRotator(0.f, -30.f, 0.f));
	TriangleCeilingBox2->SetBoxExtent(FVector(100.f, 100.f, 10.f));
	TriangleCeilingBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox2->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);
	
	TriangleCeilingBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox3"));
	TriangleCeilingBox3->SetupAttachment(GetMesh());
	TriangleCeilingBox3->SetRelativeLocation(FVector(0.f, -236.f, 0.f));
	TriangleCeilingBox3->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	TriangleCeilingBox3->SetBoxExtent(FVector(100.f, 100.f, 10.f));
	TriangleCeilingBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox3->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);

	TriangleCeilingBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox4"));
	TriangleCeilingBox4->SetupAttachment(GetMesh());
	TriangleCeilingBox4->SetRelativeLocation(FVector(-236.f, 0.f, 0.f));
	TriangleCeilingBox4->SetRelativeRotation(FRotator(0.f, 30.f, 0.f));
	TriangleCeilingBox4->SetBoxExtent(FVector(100.f, 100.f, 10.f));
	TriangleCeilingBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox4->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);
	
	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	WallBox->SetupAttachment(GetMesh());
	WallBox->SetRelativeLocation(FVector(0.f, 152.f, 148.f));
	WallBox->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox2"));
	WallBox2->SetupAttachment(GetMesh());
	WallBox2->SetRelativeLocation(FVector(0.f, -152.f, 148.f));
	WallBox2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	WallBox2->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox2->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox3"));
	WallBox3->SetupAttachment(GetMesh());
	WallBox3->SetRelativeLocation(FVector(152.f, 0.f, 148.f));
	WallBox3->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	WallBox3->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox3->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox4"));
	WallBox4->SetupAttachment(GetMesh());
	WallBox4->SetRelativeLocation(FVector(-152.f, 0.f, 148.f));
	WallBox4->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	WallBox4->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox4->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);

	StairsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StairsBox"));
	StairsBox->SetupAttachment(GetMesh());
	StairsBox->SetRelativeLocation(FVector(0.f, -14.f, 165.f));
	StairsBox->SetBoxExtent(FVector(180.f, 180.f, 180.f));
	StairsBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	StairsBox->SetCollisionResponseToChannel(ECC_STAIRS_TRACE, ECR_Block);

	CheckWallsTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckWallsTraceBox"));
	CheckWallsTraceBox->SetupAttachment(RootComponent);
	CheckWallsTraceBox->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	CheckWallsTraceBox->SetBoxExtent(FVector(154.f, 154.f, 96.f));
	CheckWallsTraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckWallsTraceBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CheckWallsTraceBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	CheckWallsTraceBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	Tags.Add(FName("AboveWall"));
}

void ASCCeiling::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(CeilingBox);
	BoxesToSnap.Add(CeilingBox2);
	BoxesToSnap.Add(CeilingBox3);
	BoxesToSnap.Add(CeilingBox4);
	BoxesToSnap.Add(WallBox);
	BoxesToSnap.Add(WallBox2);
	BoxesToSnap.Add(WallBox3);
	BoxesToSnap.Add(WallBox4);
	BoxesToSnap.Add(TriangleCeilingBox);
	BoxesToSnap.Add(TriangleCeilingBox2);
	BoxesToSnap.Add(TriangleCeilingBox3);
	BoxesToSnap.Add(TriangleCeilingBox4);
	BoxesToSnap.Add(StairsBox);

	SetSnapBoxes(BoxesToSnap);
}

void ASCCeiling::DestroyStructure(const bool bLog)
{
	Super::DestroyStructure(bLog);

	TArray<AActor*> OverlappingActors;
	CheckWallsTraceBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
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

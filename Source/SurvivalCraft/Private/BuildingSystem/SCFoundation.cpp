// Copyright Fillipe Romero

#include "BuildingSystem/SCFoundation.h"
#include "Components/BoxComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCFoundation::ASCFoundation()
{
	PrimaryActorTick.bCanEverTick = false;

	OverlapBox->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	OverlapBox->SetBoxExtent(FVector(110.f, 110.f, 32.f));

	FoundationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox"));
	FoundationBox->SetupAttachment(GetMesh());
	FoundationBox->SetRelativeLocation(FVector(0.f, 300.f, 0.f));
	FoundationBox->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	FoundationBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);
	FoundationBox->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);
	
	FoundationBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox2"));
	FoundationBox2->SetupAttachment(GetMesh());
	FoundationBox2->SetRelativeLocation(FVector(0.f, -300.f, 0.f));
	FoundationBox2->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	FoundationBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox2->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);
	
	FoundationBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox3"));
	FoundationBox3->SetupAttachment(GetMesh());
	FoundationBox3->SetRelativeLocation(FVector(300.f, 0.f, 0.f));
	FoundationBox3->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	FoundationBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox3->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);
	
	FoundationBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("FoundationBox4"));
	FoundationBox4->SetupAttachment(GetMesh());
	FoundationBox4->SetRelativeLocation(FVector(-300.f, 0.f, 0.f));
	FoundationBox4->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	FoundationBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	FoundationBox4->SetCollisionResponseToChannel(ECC_FOUNDATION_TRACE, ECR_Block);
	
	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	WallBox->SetupAttachment(GetMesh());
	WallBox->SetRelativeLocation(FVector(0.f, 150.f, 235.f));
	WallBox->SetBoxExtent(FVector(150.f, 20.f, 150.f));
	WallBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox2"));
	WallBox2->SetupAttachment(GetMesh());
	WallBox2->SetRelativeLocation(FVector(0.f, -150.f, 235.f));
	WallBox2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	WallBox2->SetBoxExtent(FVector(150.f, 20.f, 150.f));
	WallBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox2->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox3"));
	WallBox3->SetupAttachment(GetMesh());
	WallBox3->SetRelativeLocation(FVector(-150.f, 0.f, 235.f));
	WallBox3->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	WallBox3->SetBoxExtent(FVector(150.f, 20.f, 150.f));
	WallBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox3->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	WallBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox4"));
	WallBox4->SetupAttachment(GetMesh());
	WallBox4->SetRelativeLocation(FVector(150.f, 0.f, 235.f));
	WallBox4->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	WallBox4->SetBoxExtent(FVector(150.f, 20.f, 150.f));
	WallBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox4->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
	TriangleFoundationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox"));
	TriangleFoundationBox->SetupAttachment(GetMesh());
	TriangleFoundationBox->SetRelativeLocation(FVector(0.f, 235.f, 0.f));
	TriangleFoundationBox->SetRelativeRotation(FRotator(0.f, -60.f, 0.f));
	TriangleFoundationBox->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	TriangleFoundationBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);
	
	TriangleFoundationBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox2"));
	TriangleFoundationBox2->SetupAttachment(GetMesh());
	TriangleFoundationBox2->SetRelativeLocation(FVector(-235.f, 0.f, 0.f));
	TriangleFoundationBox2->SetRelativeRotation(FRotator(0.f, 30.f, 0.f));
	TriangleFoundationBox2->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	TriangleFoundationBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox2->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);
	
	TriangleFoundationBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox3"));
	TriangleFoundationBox3->SetupAttachment(GetMesh());
	TriangleFoundationBox3->SetRelativeLocation(FVector(235.f, 0.f, 0.f));
	TriangleFoundationBox3->SetRelativeRotation(FRotator(0.f, -30.f, 0.f));
	TriangleFoundationBox3->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	TriangleFoundationBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox3->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);
	
	TriangleFoundationBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleFoundationBox4"));
	TriangleFoundationBox4->SetupAttachment(GetMesh());
	TriangleFoundationBox4->SetRelativeLocation(FVector(0.f, -236.f, 0.f));
	TriangleFoundationBox4->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	TriangleFoundationBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleFoundationBox4->SetCollisionResponseToChannel(ECC_TRIANGLE_TRACE, ECR_Block);
	
	RampBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RampBox"));
	RampBox->SetupAttachment(GetMesh());
	RampBox->SetRelativeLocation(FVector(0.f, -300.f, 0.f));
	RampBox->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	RampBox->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	RampBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	RampBox->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);
	
	RampBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("RampBox2"));
	RampBox2->SetupAttachment(GetMesh());
	RampBox2->SetRelativeLocation(FVector(300.f, 0.f, 0.f));
	RampBox2->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	RampBox2->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	RampBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	RampBox2->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);
	
	RampBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("RampBox3"));
	RampBox3->SetupAttachment(GetMesh());
	RampBox3->SetRelativeLocation(FVector(-300.f, 0.f, 0.f));
	RampBox3->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	RampBox3->SetBoxExtent(FVector(150.f, 150.f, 80.f));
	RampBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	RampBox3->SetCollisionResponseToChannel(ECC_RAMP_TRACE, ECR_Block);
	
	StairsBox = CreateDefaultSubobject<UBoxComponent>(TEXT("StairsBox"));
	StairsBox->SetupAttachment(GetMesh());
	StairsBox->SetRelativeLocation(FVector(0.f, -14.f, 242.f));
	StairsBox->SetBoxExtent(FVector(180.f, 180.f, 180.f));
	StairsBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	StairsBox->SetCollisionResponseToChannel(ECC_STAIRS_TRACE, ECR_Block);

	CheckWallsTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckWallsTraceBox"));
	CheckWallsTraceBox->SetupAttachment(RootComponent);
	CheckWallsTraceBox->SetRelativeLocation(FVector(0.f, 0.f, 212.f));
	CheckWallsTraceBox->SetBoxExtent(FVector(154.f, 154.f, 96.f));
	CheckWallsTraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckWallsTraceBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CheckWallsTraceBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	CheckWallsTraceBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);
	
	Tags.Add(FName("Foundation"));
}

void ASCFoundation::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(FoundationBox);
	BoxesToSnap.Add(FoundationBox2);
	BoxesToSnap.Add(FoundationBox3);
	BoxesToSnap.Add(FoundationBox4);
	BoxesToSnap.Add(WallBox);
	BoxesToSnap.Add(WallBox2);
	BoxesToSnap.Add(WallBox3);
	BoxesToSnap.Add(WallBox4);
	BoxesToSnap.Add(TriangleFoundationBox);
	BoxesToSnap.Add(TriangleFoundationBox2);
	BoxesToSnap.Add(TriangleFoundationBox3);
	BoxesToSnap.Add(TriangleFoundationBox4);
	BoxesToSnap.Add(RampBox);
	BoxesToSnap.Add(RampBox2);
	BoxesToSnap.Add(RampBox3);
	BoxesToSnap.Add(StairsBox);

	SetSnapBoxes(BoxesToSnap);
}

void ASCFoundation::DestroyStructure()
{
	Super::DestroyStructure();

	TArray<AActor*> OverlappingActors;
	CheckWallsTraceBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
	if (OverlappingActors.Num() <= 0) return;

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor->ActorHasTag(FName("AboveFoundation")))
		{
			if (ASCBuildable* Buildable = Cast<ASCBuildable>(OverlappingActor))
			{
				Buildable->DestroyStructure();
			}
		}
	}
}

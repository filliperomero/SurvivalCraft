// Copyright Fillipe Romero

#include "BuildingSystem/SCWall.h"
#include "BuildingSystem/SCTorch.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCWall::ASCWall()
{
	PrimaryActorTick.bCanEverTick = false;

	InfoWidget->SetRelativeLocation(FVector(70.f, 0.f, -50.f));

	OverlapBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	CeilingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox"));
	CeilingBox->SetupAttachment(GetMesh());
	CeilingBox->SetRelativeLocation(FVector(0.f, -150.f, 140.f));
	CeilingBox->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	CeilingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	CeilingBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("CeilingBox2"));
	CeilingBox2->SetupAttachment(GetMesh());
	CeilingBox2->SetRelativeLocation(FVector(0.f, 150.f, 140.f));
	CeilingBox2->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	CeilingBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	CeilingBox2->SetCollisionResponseToChannel(ECC_FLOOR_TRACE, ECR_Block);

	TriangleCeilingBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox"));
	TriangleCeilingBox->SetupAttachment(GetMesh());
	TriangleCeilingBox->SetRelativeLocation(FVector(0.f, -89.f, 140.f));
	TriangleCeilingBox->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	TriangleCeilingBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);

	TriangleCeilingBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TriangleCeilingBox2"));
	TriangleCeilingBox2->SetupAttachment(GetMesh());
	TriangleCeilingBox2->SetRelativeLocation(FVector(0.f, 89.f, 140.f));
	TriangleCeilingBox2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	TriangleCeilingBox2->SetBoxExtent(FVector(150.f, 150.f, 10.f));
	TriangleCeilingBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriangleCeilingBox2->SetCollisionResponseToChannel(ECC_TRIANGLE_CEILING_TRACE, ECR_Block);

	TorchBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TorchBox"));
	TorchBox->SetupAttachment(GetMesh());
	TorchBox->SetRelativeLocation(FVector(0.f, 10.f, 0.f));
	TorchBox->SetBoxExtent(FVector(60.f, 10.f, 60.f));
	TorchBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchBox->SetCollisionResponseToChannel(ECC_TORCH_TRACE, ECR_Block);

	TorchBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TorchBox2"));
	TorchBox2->SetupAttachment(GetMesh());
	TorchBox2->SetRelativeLocation(FVector(0.f, -10.f, 0.f));
	TorchBox2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	TorchBox2->SetBoxExtent(FVector(60.f, 10.f, 60.f));
	TorchBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchBox2->SetCollisionResponseToChannel(ECC_TORCH_TRACE, ECR_Block);

	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	WallBox->SetupAttachment(GetMesh());
	WallBox->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	WallBox->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);

	CheckCeilingTraceBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckCeilingTraceBox"));
	CheckCeilingTraceBox->SetupAttachment(RootComponent);
	CheckCeilingTraceBox->SetRelativeLocation(FVector(0.f, 0.f, 135.f));
	CheckCeilingTraceBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckCeilingTraceBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CheckCeilingTraceBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	CheckCeilingTraceBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	Tags.Add(FName("AboveFoundation"));
}

void ASCWall::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(CeilingBox);
	BoxesToSnap.Add(CeilingBox2);
	BoxesToSnap.Add(TriangleCeilingBox);
	BoxesToSnap.Add(TriangleCeilingBox2);
	BoxesToSnap.Add(TorchBox);
	BoxesToSnap.Add(TorchBox2);
	BoxesToSnap.Add(WallBox);

	SetSnapBoxes(BoxesToSnap);
}

void ASCWall::DestroyStructure()
{
	Super::DestroyStructure();

	TArray<AActor*> OverlappingActors;
	CheckCeilingTraceBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
	if (OverlappingActors.Num() > 0)
	{
		for (AActor* OverlappingActor : OverlappingActors)
		{
			if (OverlappingActor->ActorHasTag(FName("AboveWall")) || OverlappingActor->ActorHasTag(FName("AboveFoundation")))
			{
				if (ASCBuildable* Buildable = Cast<ASCBuildable>(OverlappingActor))
				{
					Buildable->DestroyStructure();
				}
			}
		}
	}

	OverlapBox->GetOverlappingActors(OverlappingActors, ASCTorch::StaticClass());

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

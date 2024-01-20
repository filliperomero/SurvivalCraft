// Copyright Fillipe Romero

#include "BuildingSystem/SCWindowFrame.h"
#include "BuildingSystem/SCTorch.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCWindowFrame::ASCWindowFrame()
{
	PrimaryActorTick.bCanEverTick = false;

	InfoWidget->SetRelativeLocation(FVector(115.f, 0.f, -50.f));

	WindowBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WindowBox"));
	WindowBox->SetupAttachment(GetMesh());
	WindowBox->SetBoxExtent(FVector(80.f, 20.f, 60.f));
	WindowBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WindowBox->SetCollisionResponseToChannel(ECC_WINDOW_TRACE, ECR_Block);
	WindowBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	WallBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WallBox"));
	WallBox->SetupAttachment(GetMesh());
	WallBox->SetRelativeLocation(FVector(0.f, 0.f, 300.f));
	WallBox->SetBoxExtent(FVector(150.f, 10.f, 150.f));
	WallBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WallBox->SetCollisionResponseToChannel(ECC_WALL_TRACE, ECR_Block);
	
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
	TorchBox->SetRelativeLocation(FVector(-125.f, 10.f, 0.f));
	TorchBox->SetBoxExtent(FVector(30.f, 10.f, 60.f));
	TorchBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchBox->SetCollisionResponseToChannel(ECC_TORCH_TRACE, ECR_Block);
	TorchBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	TorchBox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("TorchBox2"));
	TorchBox2->SetupAttachment(GetMesh());
	TorchBox2->SetRelativeLocation(FVector(125.f, 10.f, 0.f));
	TorchBox2->SetBoxExtent(FVector(30.f, 10.f, 60.f));
	TorchBox2->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchBox2->SetCollisionResponseToChannel(ECC_TORCH_TRACE, ECR_Block);
	TorchBox2->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);
	
	TorchBox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("TorchBox3"));
	TorchBox3->SetupAttachment(GetMesh());
	TorchBox3->SetRelativeLocation(FVector(125.f, -10.f, 0.f));
	TorchBox3->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	TorchBox3->SetBoxExtent(FVector(30.f, 10.f, 60.f));
	TorchBox3->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchBox3->SetCollisionResponseToChannel(ECC_TORCH_TRACE, ECR_Block);
	TorchBox3->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	TorchBox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("TorchBox4"));
	TorchBox4->SetupAttachment(GetMesh());
	TorchBox4->SetRelativeLocation(FVector(-125.f, -10.f, 0.f));
	TorchBox4->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	TorchBox4->SetBoxExtent(FVector(30.f, 10.f, 60.f));
	TorchBox4->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchBox4->SetCollisionResponseToChannel(ECC_TORCH_TRACE, ECR_Block);
	TorchBox4->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Overlap);

	Tags.Add(FName("AboveFoundation"));
}

void ASCWindowFrame::BeginPlay()
{
	Super::BeginPlay();

	TArray<UBoxComponent*> BoxesToSnap;
	BoxesToSnap.Add(WindowBox);
	BoxesToSnap.Add(WallBox);
	BoxesToSnap.Add(CeilingBox);
	BoxesToSnap.Add(CeilingBox2);
	BoxesToSnap.Add(TriangleCeilingBox);
	BoxesToSnap.Add(TriangleCeilingBox2);
	BoxesToSnap.Add(TorchBox);
	BoxesToSnap.Add(TorchBox2);
	BoxesToSnap.Add(TorchBox3);
	BoxesToSnap.Add(TorchBox4);

	SetSnapBoxes(BoxesToSnap);
}

void ASCWindowFrame::DestroyStructure()
{
	Super::DestroyStructure();

	TArray<AActor*> OverlappingActors;
	WindowBox->GetOverlappingActors(OverlappingActors, ASCBuildable::StaticClass());
	
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

	TorchBox->GetOverlappingActors(OverlappingActors, ASCTorch::StaticClass());

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

	TorchBox2->GetOverlappingActors(OverlappingActors, ASCTorch::StaticClass());

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

	TorchBox3->GetOverlappingActors(OverlappingActors, ASCTorch::StaticClass());

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

	TorchBox4->GetOverlappingActors(OverlappingActors, ASCTorch::StaticClass());

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

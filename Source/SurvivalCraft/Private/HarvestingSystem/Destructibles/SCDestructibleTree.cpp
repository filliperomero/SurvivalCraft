// Copyright Fillipe Romero

#include "HarvestingSystem/Destructibles/SCDestructibleTree.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASCDestructibleTree::ASCDestructibleTree()
{
	PrimaryActorTick.bCanEverTick = false;
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetRelativeLocation(FVector(0.f, 0.f, 402.f));
	CapsuleComponent->SetRelativeScale3D(FVector(1.f, 1.f, 9.f));
	CapsuleComponent->SetSimulatePhysics(true);
	CapsuleComponent->SetMassOverrideInKg(NAME_None, 2000);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// Disable Inertia Conditioning for properly SimulatePhysics
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(CapsuleComponent);
	MeshComponent->SetRelativeScale3D(FVector(1.f, 1.f, 0.11f));
	MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -45.f));
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	TopCapsule1Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TopCapsule1Component"));
	TopCapsule1Component->SetupAttachment(CapsuleComponent);
	TopCapsule1Component->SetRelativeLocation(FVector(-4.f, 62.f, 40.f));
	TopCapsule1Component->SetRelativeRotation(FRotator(0.f, 0.f, -72.f));
	TopCapsule1Component->SetRelativeScale3D(FVector(1.f, 1.f, 0.11f));
	TopCapsule1Component->SetMassOverrideInKg(NAME_None, 400);
	TopCapsule1Component->SetCollisionResponseToAllChannels(ECR_Block);
	TopCapsule1Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TopCapsule1Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	TopCapsule2Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TopCapsule2Component"));
	TopCapsule2Component->SetupAttachment(CapsuleComponent);
	TopCapsule2Component->SetRelativeLocation(FVector(63.f, -2.f, 40.f));
	TopCapsule2Component->SetRelativeRotation(FRotator(-5.f, -93.f, -72.f));
	TopCapsule2Component->SetRelativeScale3D(FVector(1.f, 1.f, 0.11f));
	TopCapsule2Component->SetMassOverrideInKg(NAME_None, 400);
	TopCapsule2Component->SetCollisionResponseToAllChannels(ECR_Block);
	TopCapsule2Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TopCapsule2Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	TopCapsule3Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TopCapsule3Component"));
	TopCapsule3Component->SetupAttachment(CapsuleComponent);
	TopCapsule3Component->SetRelativeLocation(FVector(-60.f, -2.f, 40.f));
	TopCapsule3Component->SetRelativeRotation(FRotator(-5.f, 90.f, -72.f));
	TopCapsule3Component->SetRelativeScale3D(FVector(1.f, 1.f, 0.11f));
	TopCapsule3Component->SetMassOverrideInKg(NAME_None, 400);
	TopCapsule3Component->SetCollisionResponseToAllChannels(ECR_Block);
	TopCapsule3Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TopCapsule3Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	TopCapsule4Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TopCapsule4Component"));
	TopCapsule4Component->SetupAttachment(CapsuleComponent);
	TopCapsule4Component->SetRelativeLocation(FVector(-4.f, -57.f, 40.f));
	TopCapsule4Component->SetRelativeRotation(FRotator(-0.f, -180.f, -72.f));
	TopCapsule4Component->SetRelativeScale3D(FVector(1.f, 1.f, 0.11f));
	TopCapsule4Component->SetMassOverrideInKg(NAME_None, 400);
	TopCapsule4Component->SetCollisionResponseToAllChannels(ECR_Block);
	TopCapsule4Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TopCapsule4Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DissolveTimelineComponent"));
}

void ASCDestructibleTree::BeginPlay()
{
	Super::BeginPlay();
	
	AddForce();
	
	GetWorldTimerManager().SetTimer(DissolveTimer, this, &ThisClass::DissolveTimerFinished, DissolveDelay);
	SetLifeSpan(DestroyCooldown);
}

void ASCDestructibleTree::AddForce() const
{
	CapsuleComponent->AddForce(UKismetMathLibrary::Multiply_VectorFloat(DirectionToFall, 1500.f), NAME_None, true);
}

void ASCDestructibleTree::StartDissolve()
{
	if (DissolveMaterialInstances.Num() <= 0 || !DissolveCurve || !DissolveTimeline) return;

	for (auto& DissolveMaterialInstance : DissolveMaterialInstances)
	{
		DissolveMaterialInstance.DynamicDissolveMaterialInstance = UMaterialInstanceDynamic::Create(DissolveMaterialInstance.DissolveMaterialInstance, this);
		MeshComponent->SetMaterial(DissolveMaterialInstance.MaterialIndex, DissolveMaterialInstance.DynamicDissolveMaterialInstance);
	}

	DissolveTrack.BindDynamic(this, &ThisClass::UpdateDissolveMaterial);
	
	DissolveTimeline->AddInterpFloat(DissolveCurve, DissolveTrack);
	DissolveTimeline->Play();
}

void ASCDestructibleTree::DissolveTimerFinished()
{
	StartDissolve();
}

void ASCDestructibleTree::UpdateDissolveMaterial(float DissolveValue)
{
	for (auto& DissolveMaterialInstance : DissolveMaterialInstances)
	{
		DissolveMaterialInstance.DynamicDissolveMaterialInstance->SetScalarParameterValue(TEXT("Dissolve"), DissolveValue);
	}
}


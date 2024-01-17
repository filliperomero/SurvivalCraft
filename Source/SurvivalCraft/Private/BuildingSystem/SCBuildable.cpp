// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildable.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interfaces/StructureDamageInterface.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCBuildable::ASCBuildable()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionObjectType(ECC_STRUCTURE);

	DestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructibleMesh"));
	DestructibleMesh->SetupAttachment(RootComponent);
	DestructibleMesh->SetVisibility(false);
	DestructibleMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	DestructibleMesh->SetSimulatePhysics(false);
	DestructibleMesh->EnableClustering = false;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASCBuildable::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority()) OnTakeAnyDamage.AddDynamic(this, &ThisClass::ReceiveDamage);
}

void ASCBuildable::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if (!DamageCauser->Implements<UStructureDamageInterface>()) return;

	FDamageTiers DamageTiers = IStructureDamageInterface::Execute_GetDamageTiers(DamageCauser);

	bool bCanDamage = false;

	for (const EStructureDamageType& DamType : DamageTiers.DamageTypes)
	{
		if (DamType == StructureDamageType)
		{
			bCanDamage = true;
			break;
		}
	}

	if (!bCanDamage) return;
	
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	if (Health <= 0.f)
	{
		MulticastDestroyStructure();
	}
}

void ASCBuildable::MulticastDestroyStructure_Implementation()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false);

	DestructibleMesh->SetVisibility(true);
	DestructibleMesh->SetSimulatePhysics(true);
}

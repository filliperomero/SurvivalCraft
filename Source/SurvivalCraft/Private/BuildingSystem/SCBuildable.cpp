// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildable.h"
#include "Character/SCCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Interfaces/StructureDamageInterface.h"
#include "SurvivalCraft/SurvivalCraft.h"
#include "UI/Widget/SCBuildableInteractTextWidget.h"

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

	TextOverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TextOverlapSphere"));
	TextOverlapSphere->SetupAttachment(RootComponent);
	TextOverlapSphere->SetSphereRadius(160.f);
	TextOverlapSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	TextOverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	InfoWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InfoWidget"));
	InfoWidget->SetupAttachment(RootComponent);
	InfoWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InfoWidget->SetDrawSize(FVector2d(100.f, 100.f));
}

void ASCBuildable::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ThisClass::ReceiveDamage);
		
		TextOverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTextSphereOverlap);
		TextOverlapSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnTextSphereEndOverlap);
	}
}

void ASCBuildable::OnTextSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;

	// TODO: Create a function inside the interface for that
	if (ASCCharacter* Character = Cast<ASCCharacter>(OtherActor))
	{
		Character->ClientToggleBuildableInfoWidget(this, ESlateVisibility::Visible, bIsInteractable, bHasOptions, StructureName, OwnerName, Health, MaxHealth);
	}
}

void ASCBuildable::OnTextSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;

	// TODO: Create a function inside the interface for that
	if (ASCCharacter* Character = Cast<ASCCharacter>(OtherActor))
	{
		Character->ClientToggleBuildableInfoWidget(this, ESlateVisibility::Hidden, bIsInteractable, bHasOptions, StructureName, OwnerName, Health, MaxHealth);
	}
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

	if (Health <= 0.f) DestroyStructure();
}

void ASCBuildable::DestroyStructure()
{
	MulticastDestroyStructure();
}

void ASCBuildable::MulticastDestroyStructure_Implementation()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false);

	DestructibleMesh->SetVisibility(true);
	DestructibleMesh->SetSimulatePhysics(true);

	SetLifeSpan(LifeSpan);
}

void ASCBuildable::ClientShowInteractText_Implementation(ESlateVisibility WidgetVisibility, bool bShowInteractText, bool bShowOptionsText, const FText& InStructureName, const FText& InOwnerName, float CurrentHealth, float InMaxHealth)
{
	UUserWidget* UserWidget = InfoWidget->GetUserWidgetObject();

	if (!IsValid(UserWidget)) return;

	if (USCBuildableInteractTextWidget* BuildableInteractTextWidget = Cast<USCBuildableInteractTextWidget>(UserWidget))
	{
		BuildableInteractTextWidget->SetWidgetInfo(WidgetVisibility, bShowInteractText, bShowOptionsText, InStructureName, InOwnerName, CurrentHealth, InMaxHealth);
	}
}

// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildable.h"
#include "AdvancedSessionsLibrary.h"
#include "Character/SCCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/SCGameState.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Interfaces/StructureDamageInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SCPlayerState.h"
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
		bool bShowOptionsText = false;

		if (TribeID.IsEmpty())
		{
			FBPUniqueNetId BPUniqueNetId;
			FString UniqueIDString = FString();
					
			UAdvancedSessionsLibrary::GetUniqueNetIDFromPlayerState(Character->GetPlayerState(), BPUniqueNetId);
			UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);

			if (!UniqueIDString.IsEmpty() && !OwnerNetID.IsEmpty() && UniqueIDString.Equals(OwnerNetID))
			{
				bShowOptionsText = bHasOptions;
			}
		}
		else
		{
			if (const ASCPlayerState* PlayerState = Character->GetPlayerState<ASCPlayerState>())
			{
				if (!PlayerState->GetTribeID().IsEmpty() && !TribeID.IsEmpty() && PlayerState->GetTribeID().Equals(TribeID))
				{
					bShowOptionsText = bHasOptions;
				}
			}
		}
		Character->ClientToggleBuildableInfoWidget(this, ESlateVisibility::Visible, bIsInteractable, bShowOptionsText, StructureName, OwnerName, Health, MaxHealth);
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

	if (Health <= 0.f) DestroyStructure(true);
}

bool ASCBuildable::CanInteract(ASCCharacter* Character)
{
	bool bCanInteract = false;
	
	if (const ASCPlayerState* SCPlayerState = Character->GetPlayerState<ASCPlayerState>())
	{
		if (!GetTribeID().IsEmpty())
		{
			if (SCPlayerState->IsInTribe() && SCPlayerState->GetTribeID().Equals(GetTribeID()))
			{
				bCanInteract = true;
			}
		}
		else
		{
			FBPUniqueNetId BPUniqueNetId;
			FString UniqueIDString = FString();
					
			UAdvancedSessionsLibrary::GetUniqueNetIDFromPlayerState(Character->GetPlayerState(), BPUniqueNetId);
			UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);
			
			if (UniqueIDString.Equals(GetOwnerNetID()))
			{
				bCanInteract = true;
			}
		}
	}

	return bCanInteract;
}

void ASCBuildable::DestroyStructure(const bool bLog)
{
	MulticastDestroyStructure();

	if (bLog && !GetTribeID().IsEmpty())
	{
		if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			const FText LogMessage = FText::FromString(FString::Printf(TEXT("Your %s was destroyed!"), *StructureName.ToString()));

			const FTribeLogEntry LogEntry = ASCGameState::MakeLogEntry(LogMessage, ETribeLogColor::ETL_Red);
			SCGameState->AddLogToTribe(GetTribeID(), LogEntry);
		}
	}
}

void ASCBuildable::HandleDestroyStructure()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false);

	DestructibleMesh->SetVisibility(true);
	DestructibleMesh->SetSimulatePhysics(true);

	SetLifeSpan(LifeSpan);
}

void ASCBuildable::DemolishStructure(const FText& PlayerName)
{
	if (!bCanDemolish) return;

	if (!GetTribeID().IsEmpty())
	{
		if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			const FText LogMessage = FText::FromString(FString::Printf(TEXT("%s has demolished %s"), *PlayerName.ToString(), *StructureName.ToString()));

			const FTribeLogEntry LogEntry = ASCGameState::MakeLogEntry(LogMessage, ETribeLogColor::ETL_Red);
			SCGameState->AddLogToTribe(GetTribeID(), LogEntry);
		}
	}

	DestroyStructure(false);
}

void ASCBuildable::MulticastDestroyStructure_Implementation()
{
	HandleDestroyStructure();
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

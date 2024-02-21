// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildingComponent.h"
#include "AdvancedSessionsLibrary.h"
#include "BuildingSystem/SCBuildable.h"
#include "Camera/CameraComponent.h"
#include "Character/SCCharacter.h"
#include "Components/BoxComponent.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SCPlayerState.h"
#include "SurvivalCraft/SurvivalCraft.h"

/**
 * Improvements to make:
 * + In rare moments, the snap system overlaps with the blueprint that we are trying to snap. We probably
 *	should create a boolean which we set to true when we are snapping and we check this boolean when doing the Box Trace.
 *	If it's true, we should ignore the HitActor
 *
 * + We should improve the trace feature. Instead of using a tons of Trace Channels, we could just create one specific for structures
 *	and the func GetSnapBoxes should return a TArray of struct which has the UBoxComponent to snap and the StructureType (new Enum) that we can Snap
 */
USCBuildingComponent::USCBuildingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void USCBuildingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bBuildMode)
	{
		// TODO: Fix the StructureID
		BuildModeClient(0);
	}
	else
	{
		SetComponentTickEnabled(false);
		bShouldUpdateMaterial = true;
		
		if (IsValid(BuildablePreview)) BuildablePreview->Destroy();
	}
}

void USCBuildingComponent::ClientLaunchBuildMode_Implementation(const int32 StructureID)
{
	if (!bBuildMode)
	{
		bBuildMode = true;
		BuildModeClient(StructureID);
		SetComponentTickEnabled(true);
	}
	else
	{
		bBuildMode = false;
		bShouldUpdateMaterial = true;
		SetComponentTickEnabled(false);
		
		if (IsValid(BuildablePreview)) BuildablePreview->Destroy();
	}
}

void USCBuildingComponent::ServerSpawnBuild_Implementation(FTransform BuildTransform, FVector ClientCameraVector, FRotator ClientCameraRotation)
{
	PreviewTransform = BuildTransform;

	ASCCharacter* Character = Cast<ASCCharacter>(GetOwner());
	const int32 StructureID = Character->GetStructureIDToBuild();
	
	TArray<FCraftingItemInfo> RequiredItems;
	FCraftingItemInfo CraftingItemInfo;

	CraftingItemInfo.ItemID = StructureID;
	CraftingItemInfo.ItemQuantity = 1;
	RequiredItems.Add(CraftingItemInfo);

	const bool bHasRequiredItems = Character->GetHotbarComponent()->ContainRequiredItems(RequiredItems);

	if (bHasRequiredItems && CheckBuildPlacement(StructureID, ClientCameraVector, ClientCameraRotation))
	{
		if (ASCPlayerState* PlayerState = Character->GetPlayerState<ASCPlayerState>())
		{
			if (Character->GetHotbarComponent()->RemoveItems(RequiredItems))
			{
				ASCBuildable* SpawnedBuildable = GetWorld()->SpawnActorDeferred<ASCBuildable>(GetBuildableClass(StructureID), BuildTransform);
				if (PlayerState->IsInTribe())
				{
					SpawnedBuildable->SetTribeID(PlayerState->GetTribeID());
					SpawnedBuildable->SetOwnerName(PlayerState->GetTribeName());
				}
				else
				{
					SpawnedBuildable->SetOwnerName(PlayerState->GetPlayerNickname());
				}

				FBPUniqueNetId BPUniqueNetId;
				FString UniqueIDString = FString();
					
				UAdvancedSessionsLibrary::GetUniqueNetIDFromPlayerState(PlayerState, BPUniqueNetId);
				UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);
				
				SpawnedBuildable->SetOwnerNetID(UniqueIDString);
				SpawnedBuildable->FinishSpawning(BuildTransform);
				
				Character->ResetStructureIDToBuild();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CAN'T PLACE!!!!"))
	}

	if (IsValid(BuildablePreview)) BuildablePreview->Destroy();
}

void USCBuildingComponent::BuildModeClient(const int32 StructureID)
{
	SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetOwner()) : SCCharacter; 
	
	if (!IsValid(BuildablePreview)) SpawnBuildPreview(StructureID);
	
	FVector CameraLocation = SCCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector CameraForwardVector = SCCharacter->GetFirstPersonCameraComponent()->GetForwardVector();

	FVector StartLocation = CameraLocation + CameraForwardVector * MinBuildDistance;
	FVector EndLocation = CameraLocation + CameraForwardVector * MaxBuildDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(BuildablePreview);
			
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, UEngineTypes::ConvertToCollisionChannel(BuildablePreview->GetBuildableInfo().TraceChannel), CollisionQueryParams);
	
	FRotator Rotator;
	Rotator.Yaw = 90.f + SCCharacter->GetFirstPersonCameraComponent()->GetComponentRotation().Yaw;
	
	if (HitResult.bBlockingHit)
	{
		HitActor = HitResult.GetActor();
		HitComponent = HitResult.GetComponent();
	}
	else
	{
		HitActor = nullptr;
		HitComponent = nullptr;
	}

	PreviewTransform.SetRotation(Rotator.Quaternion());
	PreviewTransform.SetLocation(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);

	if (IsValid(BuildablePreview))
	{
		if (HitResult.bBlockingHit)
		{
			bool bIsBuildFloating = false;
			
			FTransform SnappingTransform;
			if (GetSnappingPoints(SnappingTransform))
			{
				PreviewTransform = SnappingTransform;
				bool bIsOverlapping = CheckForOverlap();
				if (BuildablePreview->GetBuildableInfo().bDoFloatCheck) bIsBuildFloating = IsBuildFloating();
				
				SetPreviewColor(!bIsOverlapping && !bIsBuildFloating);
			} 
			else
			{
				bool bCanPlace = false;
				if (BuildablePreview->GetBuildableInfo().bCanPlaceOnGround)
				{
					bool bIsOverlapping = CheckForOverlap();
					bIsBuildFloating = IsBuildFloating();
					bCanPlace = !bIsOverlapping && !bIsBuildFloating;
				}

				// A buildable could have bCanPlaceOnGround and bCanPlaceOnFoundation enabled
				if (BuildablePreview->GetBuildableInfo().bCanPlaceOnFoundation && !bCanPlace)
				{
					bool bIsOverlapping = CheckForOverlap();
					bool bIsBuildOnFoundation = IsBuildOnFoundation();
					bCanPlace = !bIsOverlapping && bIsBuildOnFoundation;
				}

				SetPreviewColor(bCanPlace);
			}
		}
		else
		{
			SetPreviewColor(false);
		}
	}
	else
	{
		SpawnBuildPreview(StructureID);
	}
}

void USCBuildingComponent::SpawnBuildPreview(const int32 StructureID)
{
	if (IsValid(BuildablePreview)) return;
	
	BuildablePreview = GetWorld()->SpawnActor<ASCBuildable>(GetBuildableClass(StructureID), PreviewTransform);
	BuildablePreview->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USCBuildingComponent::SetPreviewColor(bool bCanPlace)
{
	if (CanPlaceMaterial == nullptr || CannotPlaceMaterial == nullptr) return;

	// if (bShouldUpdateMaterial)
	// {
	// TODO: Improve this so we just update the materials when needed
	for (int32 Index = 0; Index < BuildablePreview->GetMesh()->GetNumMaterials(); Index++)
	{
		BuildablePreview->GetMesh()->SetMaterial(Index, bCanPlace ? CanPlaceMaterial : CannotPlaceMaterial);
	}
		
	// 	bShouldUpdateMaterial = false;
	// }

	BuildablePreview->GetRootComponent()->SetWorldTransform(PreviewTransform);
}

bool USCBuildingComponent::CheckForOverlap()
{
	if (!IsValid(BuildablePreview)) return false;
	
	FVector StartLocation = BuildablePreview->GetMesh()->Bounds.Origin;
	FVector HalfSize = BuildablePreview->GetMesh()->Bounds.BoxExtent / 1.2f;
	FRotator Orientation = FRotator(0.f, 0.f, 0.f);
	Orientation.Yaw = 90.f + BuildablePreview->GetRootComponent()->GetComponentRotation().Yaw;

	if (BuildablePreview->GetBuildableInfo().bUseCustomOverlap)
	{
		StartLocation = BuildablePreview->GetOverlapBox()->Bounds.Origin;
		HalfSize = BuildablePreview->GetOverlapBox()->Bounds.BoxExtent / 1.2f;
	}

	FVector EndLocation = StartLocation;

	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(BuildablePreview);

	UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		HalfSize,
		Orientation,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
	
	return HitResult.bBlockingHit;
}

bool USCBuildingComponent::GetSnappingPoints(FTransform& SnappingTransform)
{	
	if (HitActor == nullptr || HitComponent == nullptr) return false;

	// TODO: create a interface to Buildable to retrieve the SnapBoxes
	if (const ASCBuildable* HitBuildable = Cast<ASCBuildable>(HitActor))
	{
		TArray<UBoxComponent*> SnapBoxes = HitBuildable->GetSnapBoxes();

		if (SnapBoxes.Num() <= 0) return false;
		
		for (UBoxComponent*& SnapBox : SnapBoxes)
		{
			if (SnapBox == HitComponent)
			{
				SnappingTransform = HitComponent->GetComponentTransform();
				return true;
			}
		}
	}
	
	return false;
}

bool USCBuildingComponent::IsBuildFloating()
{
	if (!IsValid(BuildablePreview)) return false;
	
	FVector StartLocation = PreviewTransform.GetLocation();
	FVector EndLocation = PreviewTransform.GetLocation() - FVector(0.f, 0.f, 50.f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(BuildablePreview);

	return !GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);
}

bool USCBuildingComponent::IsBuildOnFoundation()
{
	if (!IsValid(BuildablePreview)) return false;
	
	FVector StartLocation = PreviewTransform.GetLocation();
	FVector EndLocation = PreviewTransform.GetLocation() - FVector(0.f, 0.f, 50.f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(BuildablePreview);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);

	if (HitResult.bBlockingHit && HitResult.GetActor()->ActorHasTag(FName("Foundation")))
	{
		return true;
	}

	return false;
}

TSubclassOf<ASCBuildable> USCBuildingComponent::GetBuildableClass(const int32 StructureID)
{
	check(StructuresTable)

	const FName RowName = FName(*FString::FromInt(StructureID));

	const FBuildTableInfo* BuildTableInfo = StructuresTable->FindRow<FBuildTableInfo>(RowName, TEXT(""));

	return BuildTableInfo->BuildableClass;
}

bool USCBuildingComponent::CheckBuildPlacement(const int32 StructureID, FVector ClientCameraVector, FRotator ClientCameraRotation)
{
	SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetOwner()) : SCCharacter;

	SpawnBuildPreview(StructureID);

	if (!IsValid(BuildablePreview)) return false;

	FVector ServerCameraLocation = SCCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector StartLocation = ServerCameraLocation + (ClientCameraVector * MinBuildDistance);
	FVector EndLocation = ServerCameraLocation + (ClientCameraVector * MaxBuildDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(BuildablePreview);

	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, UEngineTypes::ConvertToCollisionChannel(BuildablePreview->GetBuildableInfo().TraceChannel), CollisionQueryParams);

	FRotator Rotator = FRotator(0.f, 90.f + ClientCameraRotation.Yaw, 0.f);

	PreviewTransform.SetRotation(Rotator.Quaternion());
	PreviewTransform.SetLocation(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);

	if (!HitResult.bBlockingHit) return false;

	HitActor = HitResult.GetActor();
	HitComponent = HitResult.GetComponent();

	if (!CheckFoundationRange()) return false;

	bool bIsBuildFloating = false;

	FTransform SnappingTransform;
	if (GetSnappingPoints(SnappingTransform))
	{
		PreviewTransform = SnappingTransform;

		if (BuildablePreview->GetBuildableInfo().bDoFloatCheck) bIsBuildFloating = IsBuildFloating();
	}
	else
	{
		if (BuildablePreview->GetBuildableInfo().bCanPlaceOnGround)
		{
			bool bIsOverlapping = CheckForOverlap();
			bIsBuildFloating = IsBuildFloating();
			if (!bIsOverlapping && !bIsBuildFloating) return !bIsOverlapping && !bIsBuildFloating;
		}
		
		if (BuildablePreview->GetBuildableInfo().bCanPlaceOnFoundation)
		{
			bool bIsBuildOnFoundation = IsBuildOnFoundation();
			return !CheckForOverlap() && bIsBuildOnFoundation;
		}

		return false;
	}

	return !CheckForOverlap() && !bIsBuildFloating;
}

bool USCBuildingComponent::CheckFoundationRange()
{
	if (!IsValid(BuildablePreview)) return false;

	SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetOwner()) : SCCharacter;
	const FVector LocationToCheck = BuildablePreview->GetActorLocation();

	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_STRUCTURE));
	ActorsToIgnore.Add(BuildablePreview);

	const ASCPlayerState* SCPlayerState = SCCharacter->GetPlayerState<ASCPlayerState>();

	bool bCanPlace = true;
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), LocationToCheck, 1000.f, TraceObjects, ASCBuildable::StaticClass(), ActorsToIgnore, OutActors))
	{
		FBPUniqueNetId BPUniqueNetId;
		FString UniqueIDString = FString();
					
		UAdvancedSessionsLibrary::GetUniqueNetIDFromPlayerState(SCCharacter->GetPlayerState(), BPUniqueNetId);
		UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);
		
		for (AActor* OutActor : OutActors)
		{
			if (const ASCBuildable* Buildable = Cast<ASCBuildable>(OutActor))
			{
				if (SCPlayerState->IsInTribe() && !Buildable->GetTribeID().IsEmpty())
				{
					if (!SCPlayerState->GetTribeID().Equals(Buildable->GetTribeID()))
					{
						bCanPlace = false;
					}
				}
				else
				{
					if (UniqueIDString.IsEmpty() || Buildable->GetOwnerNetID().IsEmpty() || !UniqueIDString.Equals(Buildable->GetOwnerNetID()))
					{
						bCanPlace = false;	
					}
				}
			}

			if (!bCanPlace) break;
		}
	}

	return bCanPlace;
}

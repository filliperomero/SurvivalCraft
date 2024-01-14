﻿// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildingComponent.h"
#include "BuildingSystem/SCBuildable.h"
#include "Camera/CameraComponent.h"
#include "Character/SCCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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

void USCBuildingComponent::ServerSpawnBuild_Implementation(FTransform BuildTransform, FVector ClientCameraVector, FRotator ClientCameraRotation, int32 StructureID)
{
	PreviewTransform = BuildTransform;
	
	if (CheckBuildPlacement(StructureID, ClientCameraVector, ClientCameraRotation))
	{
		GetWorld()->SpawnActor<ASCBuildable>(GetBuildableClass(StructureID), BuildTransform);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CAN't PLACE!!!!"))
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
			FTransform SnappingTransform;
			if (GetSnappingPoints(SnappingTransform)) PreviewTransform = SnappingTransform;
			
			bool bIsOverlapping = CheckForOverlap();
			SetPreviewColor(!bIsOverlapping);
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
	for (int32 Index = 0; Index < BuildablePreview->GetMesh()->GetNumMaterials() - 1; Index++)
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
	FVector ServerCameraForwardVector = SCCharacter->GetFirstPersonCameraComponent()->GetForwardVector();
	
	FVector StartLocation = ServerCameraLocation + ClientCameraVector * MinBuildDistance;
	FVector EndLocation = ServerCameraForwardVector + ClientCameraVector * MaxBuildDistance;

	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(BuildablePreview);
		
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, UEngineTypes::ConvertToCollisionChannel(BuildablePreview->GetBuildableInfo().TraceChannel), CollisionQueryParams);

	FRotator Rotator = FRotator(0.f, 90.f + ClientCameraRotation.Yaw, 0.f);

	PreviewTransform.SetRotation(Rotator.Quaternion());
	PreviewTransform.SetLocation(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);

	if (!HitResult.bBlockingHit) return false;

	FTransform SnappingTransform;
	if (GetSnappingPoints(SnappingTransform)) PreviewTransform = SnappingTransform;

	return !CheckForOverlap();
}
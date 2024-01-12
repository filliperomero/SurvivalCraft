// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildingComponent.h"
#include "BuildingSystem/SCBuildable.h"
#include "Camera/CameraComponent.h"
#include "Character/SCCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	
	if (CheckBuildPlacement(123, ClientCameraVector, ClientCameraRotation))
	{
		GetWorld()->SpawnActor<ASCBuildable>(BuildablePreviewClass, BuildTransform);
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
			
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);
	
	FRotator Rotator;
	Rotator.Yaw = 90.f + SCCharacter->GetFirstPersonCameraComponent()->GetComponentRotation().Yaw;

	PreviewTransform.SetRotation(Rotator.Quaternion());
	PreviewTransform.SetLocation(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);

	if (IsValid(BuildablePreview))
	{
		if (HitResult.bBlockingHit)
		{
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
	
	BuildablePreview = GetWorld()->SpawnActor<ASCBuildable>(BuildablePreviewClass, PreviewTransform);
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
	
	return UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		StartLocation,
		EndLocation,
		HalfSize,
		Orientation,
		TraceTypeQuery4,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
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
		
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);

	FRotator Rotator = FRotator(0.f, 90.f + ClientCameraRotation.Yaw, 0.f);

	PreviewTransform.SetRotation(Rotator.Quaternion());
	PreviewTransform.SetLocation(HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);

	if (!HitResult.bBlockingHit) return false;

	return !CheckForOverlap();
}

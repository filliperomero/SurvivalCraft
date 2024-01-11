// Copyright Fillipe Romero

#include "BuildingSystem/SCBuildingComponent.h"
#include "BuildingSystem/SCBuildable.h"
#include "Camera/CameraComponent.h"
#include "Character/SCCharacter.h"

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

void USCBuildingComponent::SpawnBuildOnServer_Implementation(FTransform BuildTransform, FVector ClientCameraVector, FRotator ClientCameraRotation)
{
	GetWorld()->SpawnActor<ASCBuildable>(BuildablePreviewClass, BuildTransform);
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
		SetPreviewColor(false);
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

	if (bShouldUpdateMaterial)
	{
		for (int32 Index = 0; Index < BuildablePreview->GetMesh()->GetNumMaterials() - 1; Index++)
		{
			BuildablePreview->GetMesh()->SetMaterial(Index, bCanPlace ? CanPlaceMaterial : CannotPlaceMaterial);
		}
		
		bShouldUpdateMaterial = false;
	}

	BuildablePreview->GetRootComponent()->SetWorldTransform(PreviewTransform);
}

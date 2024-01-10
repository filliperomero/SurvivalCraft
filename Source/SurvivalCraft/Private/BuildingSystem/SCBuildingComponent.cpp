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
		SetComponentTickEnabled(false);
		
		if (IsValid(BuildablePreview)) BuildablePreview->Destroy();
	}
}

void USCBuildingComponent::BuildModeClient(const int32 StructureID)
{
	SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetOwner()) : SCCharacter; 
	FTransform PreviewTransform;
	
	if (!IsValid(BuildablePreview)) SpawnBuildPreview(StructureID, PreviewTransform);
	
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
		BuildablePreview->GetRootComponent()->SetWorldTransform(PreviewTransform);
	}
	else
	{
		SpawnBuildPreview(StructureID, PreviewTransform);
	}
}

void USCBuildingComponent::SpawnBuildPreview(const int32 StructureID, FTransform& PreviewTransform)
{
	if (IsValid(BuildablePreview)) return;
	
	BuildablePreview = GetWorld()->SpawnActor<ASCBuildable>(BuildablePreviewClass, PreviewTransform);
	BuildablePreview->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

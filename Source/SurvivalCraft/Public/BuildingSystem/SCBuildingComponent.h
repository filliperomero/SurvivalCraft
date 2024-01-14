// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SCBuildingComponent.generated.h"

class ASCCharacter;
class ASCBuildable;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCBuildingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Client, Reliable)
	void ClientLaunchBuildMode(const int32 StructureID);

	UFUNCTION(Server, Reliable)
	void ServerSpawnBuild(FTransform BuildTransform, FVector ClientCameraVector, FRotator ClientCameraRotation, int32 StructureID);

private:
	void BuildModeClient(const int32 StructureID);
	void SpawnBuildPreview(const int32 StructureID);
	void SetPreviewColor(bool bCanPlace);
	bool CheckForOverlap();
	bool GetSnappingPoints(FTransform& SnappingTransform);
	bool IsBuildFloating();
	TSubclassOf<ASCBuildable> GetBuildableClass(const int32 StructureID);
	// To be used by the Server
	bool CheckBuildPlacement(const int32 StructureID, FVector ClientCameraVector, FRotator ClientCameraRotation);
	
	UPROPERTY(EditAnywhere, Category = "Building Properties")
	TObjectPtr<UDataTable> StructuresTable;

	UPROPERTY(EditAnywhere, Category = "Building Properties")
	float MinBuildDistance = 350.f;

	UPROPERTY(EditAnywhere, Category = "Building Properties")
	float MaxBuildDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Building Properties")
	TObjectPtr<UMaterial> CanPlaceMaterial;

	UPROPERTY(EditAnywhere, Category = "Building Properties")
	TObjectPtr<UMaterial> CannotPlaceMaterial;
	
	UPROPERTY()
	TObjectPtr<ASCBuildable> BuildablePreview;

	UPROPERTY()
	ASCCharacter* SCCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<AActor> HitActor;
	
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> HitComponent;

	FTransform PreviewTransform;
	
	bool bBuildMode = false;
	bool bShouldUpdateMaterial = true;

public:
	FORCEINLINE bool IsBuildModeEnabled() const { return bBuildMode; }
	FORCEINLINE FTransform GetPreviewTransform() const { return PreviewTransform; }
	FORCEINLINE void SetBuildMode(const bool IsEnabled) { bBuildMode = IsEnabled; }
};

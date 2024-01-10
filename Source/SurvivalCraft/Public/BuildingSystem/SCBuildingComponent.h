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

private:
	void BuildModeClient(const int32 StructureID);
	void SpawnBuildPreview(const int32 StructureID, FTransform& PreviewTransform);
	
	// Provisory
	UPROPERTY(EditAnywhere, Category = "Building Properties")
	TSubclassOf<ASCBuildable> BuildablePreviewClass;

	UPROPERTY(EditAnywhere, Category = "Building Properties")
	float MinBuildDistance = 350.f;

	UPROPERTY(EditAnywhere, Category = "Building Properties")
	float MaxBuildDistance = 1000.f;
	
	UPROPERTY()
	TObjectPtr<ASCBuildable> BuildablePreview;

	UPROPERTY()
	ASCCharacter* SCCharacter;
	
	bool bBuildMode = false;
};

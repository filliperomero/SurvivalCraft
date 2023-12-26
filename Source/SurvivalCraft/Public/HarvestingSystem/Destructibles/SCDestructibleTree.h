// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "SCDestructibleTree.generated.h"

class UCapsuleComponent;

USTRUCT(BlueprintType)
struct FDissolveMaterial
{
	GENERATED_BODY()

	FDissolveMaterial() = default;

	UPROPERTY(EditDefaultsOnly)
	int32 MaterialIndex = 0;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance = nullptr;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance = nullptr;
};

UCLASS()
class SURVIVALCRAFT_API ASCDestructibleTree : public ASCDestructibleHarvestable
{
	GENERATED_BODY()

public:
	ASCDestructibleTree();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TopCapsule1Component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TopCapsule2Component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TopCapsule3Component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TopCapsule4Component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

private:
	void AddForce() const;

	/** Dissolve Effect */
	
	UPROPERTY(EditAnywhere, Category = "Dissolve Effect")
	float DissolveDelay = 10.f;
	
	UPROPERTY(VisibleAnywhere, Category = "Dissolve Effect")
	TObjectPtr<UTimelineComponent> DissolveTimeline;

	UPROPERTY(EditAnywhere, Category = "Dissolve Effect")
	TObjectPtr<UCurveFloat> DissolveCurve;

	UPROPERTY(EditAnywhere, Category = "Dissolve Effect")
	TArray<FDissolveMaterial> DissolveMaterialInstances;

	FOnTimelineFloat DissolveTrack;
	FTimerHandle DissolveTimer;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	
	void StartDissolve();
	void DissolveTimerFinished();
};

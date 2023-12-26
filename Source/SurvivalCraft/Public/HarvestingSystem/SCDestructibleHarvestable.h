// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "SCDestructibleHarvestable.generated.h"

class UTimelineComponent;
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
class SURVIVALCRAFT_API ASCDestructibleHarvestable : public AActor
{
	GENERATED_BODY()

public:
	ASCDestructibleHarvestable();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DestroyCooldown = 15.f;

private:
	void AddForce() const;

	UPROPERTY(Replicated)
	FVector DirectionToFall = FVector::ZeroVector;

	void DestroyTimerFinished();

	FTimerHandle DestroyTimer;

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

public:
	FORCEINLINE void SetDirectionToFall(const FVector& InDirection) { DirectionToFall = InDirection; }
};

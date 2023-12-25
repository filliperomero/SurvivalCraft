// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCDestructibleHarvestable.generated.h"

class UCapsuleComponent;

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
	float DestroyCooldown = 10.f;

private:
	void AddForce() const;

	UPROPERTY(Replicated)
	FVector DirectionToFall = FVector::ZeroVector;

	void DestroyTimerFinished();

	FTimerHandle DestroyTimer;

public:
	FORCEINLINE void SetDirectionToFall(const FVector& InDirection) { DirectionToFall = InDirection; }
};

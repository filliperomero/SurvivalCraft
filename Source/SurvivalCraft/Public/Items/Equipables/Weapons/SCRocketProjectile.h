// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCRocketProjectile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class SURVIVALCRAFT_API ASCRocketProjectile : public AActor
{
	GENERATED_BODY()

public:
	ASCRocketProjectile();

	UPROPERTY(EditAnywhere)
	float Damage = 0.f;

protected:
	void BeginPlay() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> TrailSystem;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ImpactParticles;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ImpactSound;

	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> TrailSystemComponent;
	
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;
    
	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 500.f;

private:
	void SpawnTrailSystem();
	void StartDestroyTimer();

	UPROPERTY(EditAnywhere)
	float DestroyDelay = 3.f; 

	FTimerHandle DestroyTimer;

	UFUNCTION()
	void DestroyTimerFinished();
};

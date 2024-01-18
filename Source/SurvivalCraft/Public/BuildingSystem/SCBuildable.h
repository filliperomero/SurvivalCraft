// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Data/BuildableData.h"
#include "GameFramework/Actor.h"
#include "SCBuildable.generated.h"

class UBoxComponent;

UCLASS()
class SURVIVALCRAFT_API ASCBuildable : public AActor
{
	GENERATED_BODY()

public:
	ASCBuildable();
	virtual void DestroyStructure();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDestroyStructure();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UGeometryCollectionComponent> DestructibleMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision Box")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	FBuildableInfo BuildableInfo;

	UPROPERTY(VisibleAnywhere, Category = "Collision Box" )
	TArray<UBoxComponent*> SnapBoxes;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buildable Properties", meta = (AllowPrivateAccess = true))
	EStructureDamageType StructureDamageType = EStructureDamageType::None;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	float LifeSpan = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "Buildable Properties")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	float MaxHealth = 100.f;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE FBuildableInfo GetBuildableInfo() const { return BuildableInfo; }
	FORCEINLINE UBoxComponent* GetOverlapBox() const { return OverlapBox; }
	FORCEINLINE TArray<UBoxComponent*> GetSnapBoxes() const { return SnapBoxes; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSnapBoxes(const TArray<UBoxComponent*>& InSnapBoxes) { SnapBoxes = InSnapBoxes; }
};

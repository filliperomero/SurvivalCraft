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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision Box")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	FBuildableInfo BuildableInfo;

	UPROPERTY(VisibleAnywhere, Category = "Collision Box" )
	TArray<UBoxComponent*> SnapBoxes;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE FBuildableInfo GetBuildableInfo() const { return BuildableInfo; }
	FORCEINLINE UBoxComponent* GetOverlapBox() const { return OverlapBox; }
	FORCEINLINE TArray<UBoxComponent*> GetSnapBoxes() const { return SnapBoxes; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSnapBoxes(const TArray<UBoxComponent*>& InSnapBoxes) { SnapBoxes = InSnapBoxes; }
};

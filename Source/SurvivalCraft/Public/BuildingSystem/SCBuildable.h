// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Data/BuildableData.h"
#include "GameFramework/Actor.h"
#include "SCBuildable.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCBuildable : public AActor
{
	GENERATED_BODY()

public:
	ASCBuildable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	FBuildableInfo BuildableInfo;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
};

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Data/SCItemData.h"
#include "GameFramework/Actor.h"
#include "SCItemMaster.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCItemMaster : public AActor
{
	GENERATED_BODY()

public:
	ASCItemMaster();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
	TObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	FName ItemRowName = FName();

	UPROPERTY(BlueprintReadOnly, Category = "Item Properties")
	FItemInformation ItemInfo;

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> BaseMesh;
};

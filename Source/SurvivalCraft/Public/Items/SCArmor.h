// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Items/SCItemMaster.h"
#include "Data/SCItemData.h"
#include "SCArmor.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCArmor : public ASCItemMaster
{
	GENERATED_BODY()
	
public:
	ASCArmor();

	void SetMasterPose(USkeletalMeshComponent* CharacterMesh) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor Properties")
	TObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor Properties")
	FName ItemRowName = FName();

	UPROPERTY(BlueprintReadOnly, Category = "Armor Properties")
	FItemInformation ArmorItemInfo;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
};

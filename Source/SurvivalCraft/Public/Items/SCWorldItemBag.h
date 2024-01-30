// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Data/SCItemData.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractInterface.h"
#include "SCWorldItemBag.generated.h"

class USphereComponent;

UCLASS()
class SURVIVALCRAFT_API ASCWorldItemBag : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ASCWorldItemBag();

	/** Interact Interface */
	virtual void InteractEvent_Implementation(ASCCharacter* Character) override;
	/** Interact Interface */

protected:
	void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	FItemInformation BagItemInfo;

	UPROPERTY(EditAnywhere, Category = "World Item Bag Properties")
	float DespawnBagDelay = 180.f;
	
private:
	FTimerHandle DespawnBagTimer;

	UFUNCTION()
	void DespawnBagFinished();

public:
	FORCEINLINE void SetBagItemInfo(const FItemInformation& InBagItemInfo) { BagItemInfo = InBagItemInfo; }
};

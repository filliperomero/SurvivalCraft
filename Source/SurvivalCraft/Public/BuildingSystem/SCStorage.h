// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCBuildable.h"
#include "Crafting/Data/CraftingData.h"
#include "Interfaces/InteractInterface.h"
#include "SCStorage.generated.h"

class USCStorageContainerComponent;

UCLASS()
class SURVIVALCRAFT_API ASCStorage : public ASCBuildable, public IInteractInterface
{
	GENERATED_BODY()

public:
	ASCStorage();

	/** Interact Interface */
	virtual void InteractEvent_Implementation(ASCCharacter* Character) override;
	/** Interact Interface */

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USCStorageContainerComponent> StorageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Properties")
	ECraftingType StorageType = ECraftingType::ECFT_StorageBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Properties")
	int32 StorageSize = 30;
};

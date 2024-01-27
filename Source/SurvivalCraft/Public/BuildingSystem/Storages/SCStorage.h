// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/SCBuildable.h"
#include "Crafting/Data/CraftingData.h"
#include "Interfaces/InteractInterface.h"
#include "SCStorage.generated.h"

struct FItemInformation;
enum class EContainerType : uint8;
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

	void UpdateItemSlotToAccessingCharacters(EContainerType ContainerType, int32 Index, const FItemInformation& Item); // UpdateWidgets
	void ResetItemSlotToAccessingCharacters(EContainerType ContainerType, int32 Index); // ResetItemSlot
	void UpdateStorageUI();
	void RemoveAccessingCharacter(ASCCharacter* Character);
	virtual void RunAction();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	TObjectPtr<USCStorageContainerComponent> StorageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Properties")
	ECraftingType StorageType = ECraftingType::ECFT_StorageBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Properties")
	int32 StorageSize = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Storage Properties")
	bool bIsBag = false;

	TArray<ASCCharacter*> AccessingCharacters;

private:
	bool IsSomeoneAccessing() const;

	/* To be implemented by the child's */
	virtual void OpenStorage();
	virtual void CloseStorage();

public:
	FORCEINLINE bool IsBag() const { return bIsBag; }
	FORCEINLINE USCStorageContainerComponent* GetStorageComponent() const { return StorageComponent; }
};

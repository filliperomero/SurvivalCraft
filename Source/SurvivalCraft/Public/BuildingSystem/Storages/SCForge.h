// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCStorage.h"
#include "SCForge.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCForge : public ASCStorage
{
	GENERATED_BODY()

public:
	ASCForge();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void RunAction() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VFX)
	TObjectPtr<UParticleSystemComponent> FireEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Forge Properties")
	TArray<FCraftingItemInfo> RequiredItemsToRunAction;

	UPROPERTY(EditAnywhere, Category = "Forge Properties")
	float ForgeRate = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Forge Properties")
	TObjectPtr<UDataTable> ForgeRecipesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Forge Properties")
	TObjectPtr<UDataTable> ItemsDataTable;

private:
	void ToggleFire();
	
	UPROPERTY(ReplicatedUsing = OnRep_IsFireOn)
	bool bIsFireOn = false;

	UFUNCTION()
	void OnRep_IsFireOn();
	
	FTimerHandle ForgeItemsTimer;
	
	void ForgeItemsFinished();
};

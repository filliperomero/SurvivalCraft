// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCStorage.h"
#include "SCCookingPot.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCCookingPot : public ASCStorage
{
	GENERATED_BODY()

public:
	ASCCookingPot();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void RunAction() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VFX)
	TObjectPtr<UParticleSystemComponent> FireEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking Pot Properties")
	TArray<FCraftingItemInfo> RequiredItemsToRunAction;

	UPROPERTY(EditAnywhere, Category = "Cooking Pot Properties")
	float CookingRate = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking Pot Properties")
	TObjectPtr<UDataTable> CookingPotRecipesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cooking Pot Properties")
	TObjectPtr<UDataTable> ItemsDataTable;

private:
	void ToggleFire();
	
	UPROPERTY(ReplicatedUsing = OnRep_IsFireOn)
	bool bIsFireOn = false;

	UFUNCTION()
	void OnRep_IsFireOn();
	
	FTimerHandle CookingItemsTimer;
	
	void CookingItemsFinished();
};

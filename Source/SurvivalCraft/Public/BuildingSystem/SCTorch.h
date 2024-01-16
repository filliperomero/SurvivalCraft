// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCBuildable.h"
#include "Interfaces/InteractInterface.h"
#include "SCTorch.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCTorch : public ASCBuildable, public IInteractInterface
{
	GENERATED_BODY()

public:
	ASCTorch();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Interact Interface */
	virtual void InteractEvent_Implementation(ASCCharacter* Character) override;
	/** Interact Interface */

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = VFX)
	TObjectPtr<UParticleSystemComponent> FireEffect;

private:
	void ToggleTorch();
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsTorchLit, meta = (AllowPrivateAccess = true))
	bool bIsTorchLit = false;

	UFUNCTION()
	void OnRep_IsTorchLit();
};

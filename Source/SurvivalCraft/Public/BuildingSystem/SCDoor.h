// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCBuildable.h"
#include "Interfaces/InteractInterface.h"
#include "SCDoor.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCDoor : public ASCBuildable, public IInteractInterface
{
	GENERATED_BODY()

public:
	ASCDoor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	/** Interact Interface */
	virtual void InteractEvent_Implementation(ASCCharacter* Character) override;
	/** Interact Interface */

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void RotateDoorTimeline();

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsDoorOpen, meta = (AllowPrivateAccess = true))
	bool bIsDoorOpen = false;

	UFUNCTION()
	void OnRep_IsDoorOpen();
};

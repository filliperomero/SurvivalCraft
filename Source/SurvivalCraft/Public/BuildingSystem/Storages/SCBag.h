// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/Storages/SCStorage.h"
#include "SCBag.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCBag : public ASCStorage
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Bag Properties")
	float DestroySelfDelay = 300.f;

private:
	FTimerHandle DestroySelfTimer;

	UFUNCTION()
	void DestroySelfFinished();
};

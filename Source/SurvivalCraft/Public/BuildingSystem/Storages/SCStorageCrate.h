// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/SCStorage.h"
#include "SCStorageCrate.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCStorageCrate : public ASCStorage
{
	GENERATED_BODY()

public:
	ASCStorageCrate();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> StorageLidMeshComponent;
	
	UFUNCTION(BlueprintImplementableEvent)
	void RotateLidTimeline();

private:
	virtual void OpenStorage() override;
	virtual void CloseStorage() override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_IsLidOpen, meta = (AllowPrivateAccess = true))
	bool bIsLidOpen = false;

	UFUNCTION()
	void OnRep_IsLidOpen();
};

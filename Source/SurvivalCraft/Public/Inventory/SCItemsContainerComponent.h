// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Data/SCItemData.h"
#include "SCItemsContainerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURVIVALCRAFT_API USCItemsContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCItemsContainerComponent();
	
	virtual void InitializeItems(const FItemInformation& Item, const int32 Size);
	
	UFUNCTION(BlueprintCallable) // Provisory UFUNCTION
	virtual void AddItem(FItemInformation Item);
	
	TArray<FItemInformation> Items;

protected:
	virtual void BeginPlay() override;
	
	virtual void FindEmptySlot(bool& bSuccess, int32& OutEmptyIndex);
	
	UFUNCTION(Server, Reliable)
	void ServerAddItem(FItemInformation Item);
};

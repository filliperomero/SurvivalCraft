// Copyright Fillipe Romero

#include "BuildingSystem/Storages/SCCookingPot.h"

#include "Inventory/SCStorageContainerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

ASCCookingPot::ASCCookingPot()
{
	PrimaryActorTick.bCanEverTick = false;

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(GetMesh());
	FireEffect->SetRelativeLocation(FVector(-35.f, -75.f, 5.f));
	FireEffect->SetAutoActivate(false);
}

void ASCCookingPot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCCookingPot, bIsFireOn);
}

void ASCCookingPot::RunAction()
{
	Super::RunAction();

	if (bIsFireOn)
	{
		bIsFireOn = false;
		ToggleFire();
		GetWorldTimerManager().ClearTimer(CookingItemsTimer);
	}
	else
	{
		if (GetStorageComponent()->ContainRequiredItems(RequiredItemsToRunAction))
		{
			bIsFireOn = true;
			ToggleFire();
			GetWorldTimerManager().SetTimer(CookingItemsTimer, this, &ThisClass::CookingItemsFinished, CookingRate, true);
		}
	}
}

void ASCCookingPot::ToggleFire()
{
	if (bIsFireOn)
	{
		FireEffect->Activate();
	}
	else
	{
		FireEffect->Deactivate();
	}
}

void ASCCookingPot::OnRep_IsFireOn()
{
	ToggleFire();
}

void ASCCookingPot::CookingItemsFinished()
{
	// If we don't have the requirements items to run the forge, we should stop
	// Or if we cannot remove the required items for some reason, we should stop as well
	if (!GetStorageComponent()->ContainRequiredItems(RequiredItemsToRunAction) || !GetStorageComponent()->RemoveItems(RequiredItemsToRunAction))
	{
		bIsFireOn = false;
		ToggleFire();
		GetWorldTimerManager().ClearTimer(CookingItemsTimer);
		return;
	}
	
	for (auto& Row : CookingPotRecipesDataTable->GetRowMap())
	{
		FName Name = Row.Key;
		const FCraftingRecipe* CraftingRecipe = reinterpret_cast<FCraftingRecipe*>(Row.Value);

		if (CraftingRecipe && GetStorageComponent()->ContainRequiredItems(CraftingRecipe->RequiredItems))
		{
			const FName ItemIDToCraft = FName(*FString::FromInt(CraftingRecipe->ItemID));
			const FItemInformation* ItemInformation = ItemsDataTable->FindRow<FItemInformation>(ItemIDToCraft, TEXT(""));
			
			if (ItemInformation && GetStorageComponent()->RemoveItems(CraftingRecipe->RequiredItems))
			{
				GetStorageComponent()->AddItem(*ItemInformation);
			}
		}
	}
}

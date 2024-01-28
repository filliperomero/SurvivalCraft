// Copyright Fillipe Romero

#include "BuildingSystem/Storages/SCForge.h"
#include "Inventory/SCStorageContainerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"

ASCForge::ASCForge()
{
	PrimaryActorTick.bCanEverTick = false;

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(GetMesh());
	FireEffect->SetRelativeLocation(FVector(-40.f, -30.f, 95.f));
	FireEffect->SetAutoActivate(false);
}

void ASCForge::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASCForge, bIsFireOn);
}

void ASCForge::RunAction()
{
	Super::RunAction();
	
	if (bIsFireOn)
	{
		bIsFireOn = false;
		ToggleFire();
		GetWorldTimerManager().ClearTimer(ForgeItemsTimer);
	}
	else
	{
		if (GetStorageComponent()->ContainRequiredItems(RequiredItemsToRunAction))
		{
			bIsFireOn = true;
			ToggleFire();
			GetWorldTimerManager().SetTimer(ForgeItemsTimer, this, &ThisClass::ForgeItemsFinished, ForgeRate, true);
		}
	}
}

void ASCForge::ToggleFire()
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

void ASCForge::OnRep_IsFireOn()
{
	ToggleFire();
}

void ASCForge::ForgeItemsFinished()
{
	// If we don't have the requirements items to run the forge, we should stop
	// Or if we cannot remove the required items for some reason, we should stop as well
	if (!GetStorageComponent()->ContainRequiredItems(RequiredItemsToRunAction) || !GetStorageComponent()->RemoveItems(RequiredItemsToRunAction))
	{
		bIsFireOn = false;
		ToggleFire();
		GetWorldTimerManager().ClearTimer(ForgeItemsTimer);
		return;
	}
	
	for (auto& Row : ForgeRecipesDataTable->GetRowMap())
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

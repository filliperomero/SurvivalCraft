// Copyright Fillipe Romero

#include "Items/Equipables/Tools/SCPickaxe.h"
#include "Character/SCCharacter.h"
#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "HarvestingSystem/SCLargeItem.h"
#include "Items/Data/ResourceData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCPickaxe::ASCPickaxe()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASCPickaxe::UseItem_Implementation(ASCCharacter* SCCharacter)
{
	// TODO: Better to use a Interface here, so we don't need to import SCCharacter
	// We're using the same montage as the Hatchet
	SCCharacter->PlayEquipableMontage(FName("Hatchet"));
}

void ASCPickaxe::Interact_Implementation(const FVector& LocationToCheck)
{
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_STRUCTURE));
	ActorsToIgnore.Add(GetOwner());
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), LocationToCheck, 60.f, TraceObjects, nullptr, ActorsToIgnore, OutActors))
	{
		for (const auto& Actor : OutActors)
		{
			HarvestFoliage(ToolDamage, Actor);
		}
	}
}

void ASCPickaxe::HarvestFoliage(float Damage, AActor* Target)
{
	if (!LargeItemsResourceDataTable) return;

	// TODO: Improve this so we don't need to cast, we can create a interface to get the reference of the LargeItem
	ASCLargeItem* LargeItem = Cast<ASCLargeItem>(Target);

	if (LargeItem)
	{
		LargeItem->ReceiveDamage(Damage);

		// TODO: Find a better way to identify this
		FLargeItemInfo* LargeItemInfo = LargeItemsResourceDataTable->FindRow<FLargeItemInfo>(FName(*UKismetSystemLibrary::GetDisplayName(Target)), TEXT(""));

		if (!LargeItemInfo) return;

		if (LargeItem->GetHealth() > 0.f)
		{
			// TODO: Create a Interface to Add the Harvested Item so we don't need to Cast everytime
			ASCCharacter* Character = Cast<ASCCharacter>(GetOwner());
			
			for (auto& GivenItem : LargeItemInfo->GivenItems)
			{
				const int32 Quantity = CalculateGivenQuantity(GivenItem);

				if (Quantity <= 0) continue;
				
				FResourceInfo ResourceInfo;
				ResourceInfo.ResourceID = GivenItem.ResourceID;
				ResourceInfo.Quantity = Quantity;
				ResourceInfo.PreferredToolType = GivenItem.PreferredToolType;
				
				Character->ServerAddHarvestedItem(ResourceInfo);
			}
		}
		else
		{
			const FTransform SpawnTransform = Target->GetActorTransform();
			
			ASCDestructibleHarvestable* SpawnedDestructibleHarvestable = GetWorld()->SpawnActorDeferred<ASCDestructibleHarvestable>(
				LargeItemInfo->DestructibleHarvestableClass,
				SpawnTransform,
				this,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn
			);
			
			Target->Destroy();

			SpawnedDestructibleHarvestable->SetDirectionToFall(GetOwner()->GetActorForwardVector());
			SpawnedDestructibleHarvestable->FinishSpawning(SpawnTransform);
		}
	}
}

int32 ASCPickaxe::CalculateGivenQuantity(const FResourceInfo& Resource) const
{
	const float BaseQuantity = static_cast<float>(Resource.Quantity);
	constexpr float ServerRate = 1.f; // TODO: It'll be changed by a Global Variable in the future
	float ToolTypeRate = 0.f;
	float ToolTierRate = 0.f;

	if (Resource.PreferredToolType == ToolType) ToolTypeRate = FMath::FRandRange(0.2, 0.4);
	else ToolTypeRate = FMath::FRandRange(0.01, 0.1);
	
	switch (ToolTier)
	{
	case EToolTier::ETT_Stone:
		ToolTierRate = FMath::FRandRange(0.8, 1.2);
		break;
	case EToolTier::ETT_Iron:
		ToolTierRate = FMath::FRandRange(1.2, 1.6);
		break;
	}

	return FMath::TruncToInt32(BaseQuantity * ServerRate * ToolTypeRate * ToolTierRate * ToolDamage);
}

// Copyright Fillipe Romero

#include "Items/Equipables/Tools/SCHatchet.h"
#include "Character/SCCharacter.h"
#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "HarvestingSystem/SCLargeItem.h"
#include "Items/Data/ResourceData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCHatchet::ASCHatchet()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASCHatchet::UseItem_Implementation(ASCCharacter* SCCharacter)
{
	// TODO: Better to use a Interface here, so we don't need to import SCCharacter
	SCCharacter->PlayEquipableMontage(FName("Hatchet"));
}

void ASCHatchet::Interact_Implementation(const FVector& LocationToCheck)
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
			HarvestFoliage(15.f, Actor);
		}
	}
	// For Debug
	// DrawDebugSphere(GetWorld(), LocationToCheck, 60.f, 12, FColor::Red, true, 10.f, 0, 1.f);
}

void ASCHatchet::HarvestFoliage(float Damage, AActor* Target)
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
				Character->ServerAddHarvestedItem(GivenItem);
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

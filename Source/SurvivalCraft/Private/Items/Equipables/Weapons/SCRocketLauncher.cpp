// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCRocketLauncher.h"
#include "Character/SCCharacter.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Inventory/SCPlayerInventoryComponent.h"
#include "Items/Equipables/Weapons/SCRocketProjectile.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/SCPlayerController.h"

ASCRocketLauncher::ASCRocketLauncher()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(RootComponent);
}

void ASCRocketLauncher::UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	const FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];

	if (EquippedItem.CurrentAmmo >= 1)
	{
		SCCharacter->PlayEquipableMontage(FName("RocketLauncher"));
		MulticastPlayWeaponAnim();

		// TODO: Remove the sound of the FireAnimation and play here directly (remember the attenuation)
		FTransform SocketTransform = WeaponSkeletalMesh->GetSocketTransform(FName("MuzzleFlash"), RTS_World);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		ASCRocketProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ASCRocketProjectile>(RocketProjectileClass, SocketTransform.GetLocation(), ClientCameraRotation, SpawnParams);
		SpawnedProjectile->Damage = EquippedItem.ItemDamage;
		
		SpendRound(SCCharacter);
	}
}

void ASCRocketLauncher::ReloadItem_Implementation(ASCCharacter* SCCharacter)
{
	SCCharacter->SetCombatState(ECombatState::ECS_Reloading);
	
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	const FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];
	
	if (EquippedItem.CurrentAmmo < EquippedItem.MaxAmmo)
	{
		int32 ItemIndex;
		FItemInformation AmmoItem;
		if (SCCharacter->GetPlayerInventoryComponent()->FindItem(UKismetStringLibrary::Conv_StringToInt(*AmmoItemID.ToString()), ItemIndex, AmmoItem))
		{
			SCCharacter->PlayEquipableMontage(FName("ReloadRocketLauncher"));
		}
		else
		{
			SCCharacter->SetCombatState(ECombatState::ECS_Unoccupied);
		}
	}
	else
	{
		SCCharacter->SetCombatState(ECombatState::ECS_Unoccupied);
	}
}

void ASCRocketLauncher::Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation)
{
	ASCCharacter* SCCharacter = Cast<ASCCharacter>(GetOwner());

	if (!IsValid(SCCharacter)) return;

	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];

	const int32 CurrentAmmo = EquippedItem.CurrentAmmo;
	const int32 MaxAmmo = EquippedItem.MaxAmmo;

	if (CurrentAmmo < MaxAmmo)
	{
		int32 ItemIndex;
		FItemInformation AmmoItem;
		if (SCCharacter->GetPlayerInventoryComponent()->FindItem(UKismetStringLibrary::Conv_StringToInt(*AmmoItemID.ToString()), ItemIndex, AmmoItem))
		{
			int32 RoomInMag = MaxAmmo - CurrentAmmo;
			int32 Least = FMath::Min(RoomInMag, AmmoItem.ItemQuantity);
			int32 AmountToReload = FMath::Clamp(RoomInMag, 0, Least);

			if (SCCharacter->GetPlayerInventoryComponent()->RemoveItemQuantity(ItemIndex, AmountToReload))
			{
				EquippedItem.CurrentAmmo += AmountToReload;
				SCCharacter->GetHotbarComponent()->Items[EquippedItemIndex] = EquippedItem;
				SCCharacter->Execute_GetSCPlayerController(SCCharacter)->ClientUpdateItemSlot(EContainerType::ECT_PlayerHotbar, EquippedItemIndex, EquippedItem);

				SCCharacter->ServerAddToItemDurability(-2);
			}
		}
	}
}

void ASCRocketLauncher::SpendRound(ASCCharacter* SCCharacter)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];

	EquippedItem.CurrentAmmo -= 1;

	SCCharacter->GetHotbarComponent()->Items[EquippedItemIndex] = EquippedItem;

	SCCharacter->Execute_GetSCPlayerController(SCCharacter)->ClientUpdateItemSlot(EContainerType::ECT_PlayerHotbar, EquippedItemIndex, EquippedItem);
}

void ASCRocketLauncher::MulticastPlayWeaponAnim_Implementation()
{
	if (FireAnimation)
		WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
}

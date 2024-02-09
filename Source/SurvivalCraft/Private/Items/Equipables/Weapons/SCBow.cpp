// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCBow.h"
#include "Character/SCCharacter.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Inventory/SCPlayerInventoryComponent.h"
#include "Items/Equipables/Weapons/SCArrowProjectile.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/SCPlayerController.h"

ASCBow::ASCBow()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
	WeaponSkeletalMesh->SetupAttachment(RootComponent);

	ArrowStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowStaticMesh"));
	ArrowStaticMesh->SetupAttachment(WeaponSkeletalMesh, FName("ArrowSocket"));
	ArrowStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASCBow::UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	const FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];
	
	if (EquippedItem.CurrentAmmo >= 1)
	{
		SCCharacter->PlayEquipableMontage(FName("Bow"));
		if (ASCBow* FPBow = Cast<ASCBow>(SCCharacter->GetFPEquippedItem()))
		{
			FPBow->PlayFireAnimation(false);
		}
		
		PlayFireAnimation(false);
		MulticastPlayWeaponAnim(SCCharacter);
	}
}

void ASCBow::LeftButtonReleased_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	const FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];

	if (EquippedItem.CurrentAmmo >= 1)
	{
		SCCharacter->PlayEquipableMontage(FName("BowShoot"));
		if (ASCBow* FPBow = Cast<ASCBow>(SCCharacter->GetFPEquippedItem()))
		{
			FPBow->PlayFireAnimation(true);
		}
		
		PlayFireAnimation(true);
		MulticastResetWeaponAnim(SCCharacter);

		FTransform SocketTransform = WeaponSkeletalMesh->GetSocketTransform(FName("ReleaseArrowSocket"), RTS_World);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		ASCArrowProjectile* SpawnedProjectile = GetWorld()->SpawnActor<ASCArrowProjectile>(ArrowProjectileClass, SocketTransform.GetLocation(), ClientCameraRotation, SpawnParams);
		SpawnedProjectile->Damage = EquippedItem.ItemDamage;
		
		SpendRound(SCCharacter);
	}
}

void ASCBow::ReloadItem_Implementation(ASCCharacter* SCCharacter)
{
	SCCharacter->SetCombatState(ECombatState::ECS_Reloading);
	
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

				SCCharacter->ServerAddToItemDurability(-1);
			}
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

void ASCBow::MulticastResetWeaponAnim_Implementation(ASCCharacter* SCCharacter)
{
	PlayFireAnimation(true);
	if (ASCBow* FPBow = Cast<ASCBow>(SCCharacter->GetFPEquippedItem()))
	{
		FPBow->PlayFireAnimation(true);
	}
}

void ASCBow::SpendRound(ASCCharacter* SCCharacter)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];

	EquippedItem.CurrentAmmo -= 1;

	SCCharacter->GetHotbarComponent()->Items[EquippedItemIndex] = EquippedItem;

	SCCharacter->Execute_GetSCPlayerController(SCCharacter)->ClientUpdateItemSlot(EContainerType::ECT_PlayerHotbar, EquippedItemIndex, EquippedItem);
}

void ASCBow::PlayFireAnimation(bool bReset)
{
	if (FireAnimation && !bReset)
	{
		WeaponSkeletalMesh->PlayAnimation(FireAnimation, false);
		ArrowStaticMesh->SetVisibility(true);
	}

	if (bReset)
	{
		WeaponSkeletalMesh->PlayAnimation(nullptr, false);
		ArrowStaticMesh->SetVisibility(false);
	}
}

void ASCBow::MulticastPlayWeaponAnim_Implementation(ASCCharacter* SCCharacter)
{
	PlayFireAnimation(false);
	if (ASCBow* FPBow = Cast<ASCBow>(SCCharacter->GetFPEquippedItem()))
	{
		FPBow->PlayFireAnimation(false);
	}
}

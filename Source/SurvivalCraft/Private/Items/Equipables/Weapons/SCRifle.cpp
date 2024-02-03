// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCRifle.h"
#include "Camera/CameraComponent.h"
#include "Character/SCCharacter.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Inventory/SCPlayerInventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "Player/SCPlayerController.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCRifle::ASCRifle()
{
	PrimaryActorTick.bCanEverTick = false;

	RifleSkeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RifleSkeletal"));
	RifleSkeletal->SetupAttachment(RootComponent);
}

void ASCRifle::UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	const FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];
	
	if (EquippedItem.CurrentAmmo >= 1)
	{
		MulticastPlayWeaponAnim();

		const FVector StartLocation = SCCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector EndLocation = StartLocation + (UKismetMathLibrary::GetForwardVector(ClientCameraRotation) * BulletDistance);

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = true;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(SCCharacter);
		QueryParams.bReturnPhysicalMaterial = true;
		
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Camera, QueryParams);
		// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, 10.f);

		if (HitResult.bBlockingHit)
		{
			// In the course we check if the Actor has the tag "Damageable"
			float Damage = static_cast<float>(ItemInfo.ItemDamage) * DamageMultiplier;

			if (HitResult.GetActor()->Implements<UPlayerInterface>())
			{
				switch (HitResult.PhysMaterial->SurfaceType)
				{
					case SURFACE_HEAD:
						Damage *= 8.f;
						break;
					case SURFACE_CHEST:
						Damage *= 5.f;
						break;
					case SURFACE_LEGS:
						Damage *= 3.f;
						break;
					// No change on the Damage for Boots, Limb and Default
					case SURFACE_BOOTS:
					case SURFACE_LIMB:
					default: 
						break;
				}

				if (IPlayerInterface::Execute_HasArmorInSlot(HitResult.GetActor(), HitResult.PhysMaterial->SurfaceType))
				{
					Damage -= Damage * 0.25f;
				}
			}
			else
			{
				// It is something else
			}
			
			UGameplayStatics::ApplyDamage(HitResult.GetActor(), Damage, SCCharacter->GetController(), this, UDamageType::StaticClass());
		}

		SpendRound(SCCharacter);
	}
}

void ASCRifle::ReloadItem_Implementation(ASCCharacter* SCCharacter)
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
			SCCharacter->PlayEquipableMontage(FName("ReloadRifle"));
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

void ASCRifle::Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation)
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

				SCCharacter->ServerAddToItemDurability(-1);
			}
		}
	}
}

void ASCRifle::SpendRound(ASCCharacter* SCCharacter)
{
	const int32 EquippedItemIndex = SCCharacter->GetEquippedItemIndex();
	FItemInformation EquippedItem = SCCharacter->GetHotbarComponent()->GetItems()[EquippedItemIndex];

	EquippedItem.CurrentAmmo -= 1;

	SCCharacter->GetHotbarComponent()->Items[EquippedItemIndex] = EquippedItem;

	SCCharacter->Execute_GetSCPlayerController(SCCharacter)->ClientUpdateItemSlot(EContainerType::ECT_PlayerHotbar, EquippedItemIndex, EquippedItem);
}

void ASCRifle::MulticastPlayWeaponAnim_Implementation()
{
	if (FireAnimation)
		RifleSkeletal->PlayAnimation(FireAnimation, false);
}

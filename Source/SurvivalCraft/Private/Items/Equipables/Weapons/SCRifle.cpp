// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCRifle.h"
#include "Camera/CameraComponent.h"
#include "Character/SCCharacter.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	
	if (EquippedItem.CurrentAmmo > 1)
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
	}
}

void ASCRifle::MulticastPlayWeaponAnim_Implementation()
{
	if (FireAnimation)
		RifleSkeletal->PlayAnimation(FireAnimation, false);
}

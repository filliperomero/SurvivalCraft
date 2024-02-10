// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCSpear.h"
#include "SCBlueprintFunctionLibrary.h"
#include "BuildingSystem/SCBuildable.h"
#include "Character/SCCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/SCPlayerController.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCSpear::ASCSpear()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASCSpear::UseItem_Implementation(ASCCharacter* SCCharacter, FRotator ClientCameraRotation)
{
	SCCharacter->PlayEquipableMontage(FName("Spear"));
}

void ASCSpear::Interact_Implementation(const FVector& LocationToCheck, const FRotator& Rotation)
{
	ASCCharacter* SCCharacter = Cast<ASCCharacter>(GetOwner());

	if (!IsValid(SCCharacter)) return;
	
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;

	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_STRUCTURE));
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ActorsToIgnore.Add(GetOwner());
	
	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), LocationToCheck, 60.f, TraceObjects, nullptr, ActorsToIgnore, OutActors))
	{
		for (const auto& Actor : OutActors)
		{
			const float Damage = USCBlueprintFunctionLibrary::CalculateDamage(Actor, static_cast<float>(ItemInfo.ItemDamage));

			// TODO: Create a interface for Damaging the Buildable
			if (Actor->Implements<UPlayerInterface>() || Cast<ASCBuildable>(Actor))
			{
				ASCPlayerController* SCController = IPlayerInterface::Execute_GetSCPlayerController(GetOwner());
				
				UGameplayStatics::ApplyDamage(Actor, Damage, SCController, this, UDamageType::StaticClass());
				SCCharacter->ServerAddToItemDurability(-2);
			}
		}
	}
}

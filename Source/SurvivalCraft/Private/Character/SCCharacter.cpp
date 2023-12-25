﻿// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Character/SCCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Inventory/SCPlayerInventoryComponent.h"
#include "Items/SCEquipableItem.h"
#include "Items/Data/ResourceData.h"
#include "Net/UnrealNetwork.h"
#include "Player/SCPlayerController.h"
#include "UI/HUD/SCHUD.h"

ASCCharacter::ASCCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GetMesh()->SetOwnerNoSee(true);
		
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	Camera->bUsePawnControlRotation = true;

	PlayerArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerArrow"));
	PlayerArrow->SetupAttachment(Camera);
	PlayerArrow->SetRelativeLocation(FVector(100.f, 0.f, -10.f));
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(Camera);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(0.f, -90.0f, 0.f));
	Mesh1P->SetRelativeLocation(FVector(00.f, 0.f, -160.f));

	InventoryComponent = CreateDefaultSubobject<USCPlayerInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->ContainerType = EContainerType::ECT_PlayerInventory;
	
	HotbarComponent = CreateDefaultSubobject<USCPlayerHotbarComponent>(TEXT("HotbarComponent"));
	HotbarComponent->ContainerType = EContainerType::ECT_PlayerHotbar;
}

ASCPlayerController* ASCCharacter::GetSCPlayerController_Implementation()
{
	// TODO: create a variable for it and return it
	return GetController<ASCPlayerController>();
}

void ASCCharacter::OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType FromContainer, int32 FromIndex, int32 ToIndex, EArmorType ArmorType)
{
	// TODO: should we put a HasAuthority here?
	
	switch (TargetContainer) {
	case EContainerType::ECT_PlayerInventory:
		InventoryComponent->OnSlotDrop(InventoryComponent, FromIndex, ToIndex);
		break;
	case EContainerType::ECT_PlayerHotbar:
		break;
	case EContainerType::ECT_PlayerStorage:
		break;
	case EContainerType::ECT_PlayerArmor:
		break;
	}
}

void ASCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	const FItemInformation Item = FItemInformation();
	// Initialize Inventory
	InventoryComponent->InitializeItems(Item, 30);
	// Initialize Hotbar
	HotbarComponent->InitializeItems(Item, 8);
}

void ASCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// In multiplayer, the PlayerController will be invalid in other people computers, since I can only get my PlayerController but not others
	if (ASCPlayerController* SCPlayerController = Cast<ASCPlayerController>(GetController()))
	{
		// The HUD is only valid to locally controller player
		if (ASCHUD* HUD = Cast<ASCHUD>(SCPlayerController->GetHUD()))
		{
			HUD->InitOverlay(SCPlayerController, GetPlayerState());
		}
	}
}

void ASCCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// In multiplayer, the PlayerController will be invalid in other people computers, since I can only get my PlayerController but not others
	if (ASCPlayerController* SCPlayerController = Cast<ASCPlayerController>(GetController()))
	{
		// The HUD is only valid to locally controller player
		if (ASCHUD* HUD = Cast<ASCHUD>(SCPlayerController->GetHUD()))
		{
			HUD->InitOverlay(SCPlayerController, GetPlayerState());
		}
	}
}

void ASCCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCCharacter, EquipableState);
}

void ASCCharacter::UseHotBar(const int32 Index)
{
	ServerUseHotBar(Index);
}

void ASCCharacter::UseEquipable()
{
	// Improvement: We should replicate bCanUseEquipable so we can check this locally instead of checking in the server. This would save improve the lag
	ServerUseEquipable();
}

void ASCCharacter::FinishEquipable()
{
	bCanUseEquipable = true;
}

void ASCCharacter::EquipableHit()
{
	if (!HasAuthority()) return;

	IEquipableInterface::Execute_Interact(EquippedItem, PlayerArrow->GetComponentLocation());
}

void ASCCharacter::ServerUseHotBar_Implementation(const int32 Index)
{
	EItemType ItemType;
	if (HotbarComponent->HasItemInSlot(Index, ItemType))
	{
		switch (ItemType) {
		case EItemType::EIT_Resource:
			break;
		case EItemType::EIT_Equipable:
			if (IsValid(EquippedItem))
			{
				EquippedItem->Destroy();
				EquipableState = EEquipableState::EES_Default;

				ClientUnequipEquipable();
			}
			else
			{
				const FItemInformation& Item = HotbarComponent->Items[Index]; 
				SpawnEquipable(Item.ItemClass, Item, Index);
			}
			break;
		case EItemType::EIT_Armor:
			break;
		case EItemType::EIT_Consumable:
			break;
		case EItemType::EIT_Buildable:
			break;
		}
	}
}

void ASCCharacter::SpawnEquipable(TSubclassOf<AActor> EquipableItemClass, FItemInformation ItemInformation, int32 EquippedIndex)
{
	EquippedItem = GetWorld()->SpawnActor<ASCEquipableItem>(EquipableItemClass);
	EquippedItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, EquippedItem->GetEquipableItemInfo().SocketName);
	EquippedItem->SetOwner(this);
	
	EquipableState = EquippedItem->GetEquipableItemInfo().EquipableState;
	
	// MulticastEquipItem(EquippedItem, EquippedItem->GetEquipableItemInfo().SocketName, EquippedItem->GetEquipableItemInfo().EquipableState);
	ClientSpawnEquipable(EquippedItem->GetEquipableItemInfo().FirsPersonEquipClass, EquippedItem->GetEquipableItemInfo().SocketName);
}

void ASCCharacter::ClientSpawnEquipable_Implementation(TSubclassOf<ASCEquipableItem> EquipableItemClass, FName SocketName)
{
	FP_EquippedItem = GetWorld()->SpawnActor<ASCEquipableItem>(EquipableItemClass);
	FP_EquippedItem->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
}

void ASCCharacter::ClientUnequipEquipable_Implementation()
{
	if (!IsValid(FP_EquippedItem)) return;

	FP_EquippedItem->Destroy();
}

void ASCCharacter::ServerUseEquipable_Implementation()
{
	if (!IsValid(EquippedItem) || !bCanUseEquipable) return;

	bCanUseEquipable = false;

	// TODO: since useItem for now only play the montage, we can have a interface to get the MontageName and just call the RPC's from here.
	EquippedItem->UseItem_Implementation(this);
}

void ASCCharacter::PlayEquipableMontage(FName SectionName)
{
	// TODO: We can refactor this later by using a state as a enum type. Then using it with a OnRep so it'll run in other client's
	MulticastPlayEquipableMontage(SectionName);
}

void ASCCharacter::ServerAddHarvestedItem_Implementation(const FResourceInfo& Resource)
{
	if (ItemsDataTable == nullptr) return;

	if (FItemInformation* ItemInformation = ItemsDataTable->FindRow<FItemInformation>(Resource.ResourceID, TEXT("")))
	{
		ItemInformation->ItemQuantity = Resource.Quantity;

		InventoryComponent->AddItem(*ItemInformation);
	}
}

void ASCCharacter::MulticastPlayEquipableMontage_Implementation(FName SectionName)
{
	if (!EquipableMontage) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* AnimInstance1P = GetMesh1P()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(EquipableMontage);
		AnimInstance->Montage_JumpToSection(SectionName);
	}

	if (AnimInstance1P)
	{
		AnimInstance1P->Montage_Play(EquipableMontage);
		AnimInstance1P->Montage_JumpToSection(SectionName);
	}
}


// TODO: Removed because the EquippedItem (ItemMaster) is already replicated.
// void ASCCharacter::MulticastEquipItem_Implementation(AActor* Target, FName SocketName, EEquipableState InEquippedState)
// {
// 	if (!IsValid(Target) || !GetMesh()) return;
//
// 	Target->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
// 	
// 	EquipableState = InEquippedState;
// }

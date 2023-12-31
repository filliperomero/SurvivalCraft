// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Character/SCCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "HarvestingSystem/SCDestructibleHarvestable.h"
#include "HarvestingSystem/SCGroundItem.h"
#include "Inventory/SCPlayerHotbarComponent.h"
#include "Inventory/SCPlayerInventoryComponent.h"
#include "Items/SCEquipableItem.h"
#include "Items/Data/ResourceData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/SCPlayerController.h"
#include "SurvivalCraft/SurvivalCraft.h"
#include "UI/HUD/SCHUD.h"

ASCCharacter::ASCCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Block);

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

void ASCCharacter::OnSlotDrop_Implementation(EContainerType TargetContainerType, EContainerType FromContainerType, int32 FromIndex, int32 ToIndex, EArmorType ArmorType)
{
	ServerOnSlotDrop(TargetContainerType, FromContainerType, FromIndex, ToIndex, ArmorType);
}

void ASCCharacter::ServerOnSlotDrop_Implementation(EContainerType TargetContainerType, EContainerType FromContainerType, int32 FromIndex, int32 ToIndex, EArmorType ArmorType)
{
	USCItemsContainerComponent* FromContainer = nullptr;

	switch (FromContainerType) {
	case EContainerType::ECT_PlayerInventory:
		FromContainer = InventoryComponent;
		break;
	case EContainerType::ECT_PlayerHotbar:
		FromContainer = HotbarComponent;
		break;
	case EContainerType::ECT_PlayerStorage:
		break;
	case EContainerType::ECT_PlayerArmor:
		break;
	}
	
	switch (TargetContainerType) {
	case EContainerType::ECT_PlayerInventory:
		InventoryComponent->OnSlotDrop(FromContainer, FromIndex, ToIndex);
		break;
	case EContainerType::ECT_PlayerHotbar:
		HotbarComponent->OnSlotDrop(FromContainer, FromIndex, ToIndex);
		break;
	case EContainerType::ECT_PlayerStorage:
		break;
	case EContainerType::ECT_PlayerArmor:
		break;
	}
}

void ASCCharacter::ServerCraftItem_Implementation(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	// TODO: Need to create a variable called isCrafting. Probably I can use the CombatState for that
	USCItemsContainerComponent* ContainerComponent = GetContainerComponent(ContainerType);
	
	const FName RowName = FName(*FString::FromInt(ItemID));

	if (!PlayerCraftingRecipesDataTable || !ItemsDataTable) return;
	
	switch (TableType)
	{
	case ECraftingType::ECFT_PlayerInventory:
		{
			if (const FCraftingRecipe* CraftingRecipe = PlayerCraftingRecipesDataTable->FindRow<FCraftingRecipe>(RowName, TEXT("")))
			{
				if (ContainerComponent->ContainRequiredItems(CraftingRecipe->RequiredItems))
				{
					if (CraftingRecipe->CraftingTime <= 0)
					{
						CraftTimerFinished(ContainerComponent, *CraftingRecipe);
					}
					else
					{
						CraftTimerDelegate.BindUFunction(this, FName("CraftTimerFinished"), ContainerComponent, *CraftingRecipe);
						GetWorldTimerManager().SetTimer(CraftTimerHandle, CraftTimerDelegate, CraftingRecipe->CraftingTime, false);
					}
				}
				else
				{
					// We never hit here but... we never know. we should reset the combat state here. so we can craft again.
				}
			}
			break;
		}
	case ECraftingType::ECFT_CookingPot:
		break;
	case ECraftingType::ECFT_CraftingBench:
		break;
	case ECraftingType::ECFT_Forge:
		break;
	case ECraftingType::ECFT_AdvancedWorkbench:
		break;
	case ECraftingType::ECFT_StorageBox:
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
	DOREPLIFETIME(ASCCharacter, CombatState);
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

void ASCCharacter::Interact()
{
	if (CombatState == ECombatState::ECS_Unoccupied) ServerInteract();
}

bool ASCCharacter::CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	USCItemsContainerComponent* ContainerComponent = GetContainerComponent(ContainerType);
	
	if (!ContainerComponent) return false;

	const FName RowName = FName(*FString::FromInt(ItemID));
	
	switch (TableType)
	{
	case ECraftingType::ECFT_PlayerInventory:
		{
			if (PlayerCraftingRecipesDataTable)
			{
				if (const FCraftingRecipe* CraftingRecipe = PlayerCraftingRecipesDataTable->FindRow<FCraftingRecipe>(RowName, TEXT("")))
				{
					return ContainerComponent->ContainRequiredItems(CraftingRecipe->RequiredItems);	
				}
			}
			break;
		}
	case ECraftingType::ECFT_CookingPot:
		break;
	case ECraftingType::ECFT_CraftingBench:
		break;
	case ECraftingType::ECFT_Forge:
		break;
	case ECraftingType::ECFT_AdvancedWorkbench:
		break;
	case ECraftingType::ECFT_StorageBox:
		break;
	}

	return false;
}

void ASCCharacter::FinishEquipable()
{
	bCanUseEquipable = true;
}

void ASCCharacter::FinishHarvesting()
{
	if (HasAuthority()) CombatState = ECombatState::ECS_Unoccupied;
}

void ASCCharacter::EquipableHit()
{
	if (!HasAuthority()) return;

	IEquipableInterface::Execute_Interact(EquippedItem, PlayerArrow->GetComponentLocation(), PlayerArrow->GetComponentRotation());
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
				EquippedItemIndex = -1;
				EquipableState = EEquipableState::EES_Default;

				ClientUnequipEquipable();
			}
			else
			{
				const FItemInformation& Item = HotbarComponent->Items[Index];
				EquippedItemIndex = Index;
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

void ASCCharacter::OnRep_CombatState()
{
	switch (CombatState) {
	case ECombatState::ECS_Unoccupied:
		break;
	case ECombatState::ECS_Harvesting:
		PlayHarvestingMontage();
		if (HarvestBushEffect)
			UGameplayStatics::SpawnEmitterAtLocation(this, HarvestBushEffect, GetActorLocation());

		if (HarvestBushSound)
			UGameplayStatics::SpawnSoundAtLocation(this, HarvestBushSound, GetActorLocation());
		
		break;
	case ECombatState::ECS_MAX:
		break;
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

void ASCCharacter::ServerUnequipCurrentItem_Implementation(int32 Index)
{
	if (Index != EquippedItemIndex || !IsValid(EquippedItem)) return;

	EquippedItem->Destroy();
	EquipableState = EEquipableState::EES_Default;
	EquippedItemIndex = -1;
	ClientUnequipEquipable();
}

void ASCCharacter::ClientShowItemAdded_Implementation(UTexture2D* ItemIcon, int32 ItemQuantity, const FText& ItemName)
{
	// TODO: Create a local variable where we can just get from it or cast if needed
	if (ASCPlayerController* SCPC = Cast<ASCPlayerController>(GetController()))
	{
		SCPC->ShowItemAdded(ItemIcon, ItemQuantity, ItemName);
	}
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

void ASCCharacter::ServerInteract_Implementation()
{
	if (IsValid(EquippedItem)) return;
	
	TArray<AActor*> ActorsToIgnore;
	TArray<AActor*> OutActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjects;
	
	TraceObjects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ActorsToIgnore.Add(this);
	
	if (UKismetSystemLibrary::SphereOverlapActors(this, GetActorLocation(), 70.f, TraceObjects, nullptr, ActorsToIgnore, OutActors))
	{
		// TODO: We need to improve this to check the first actor and set the combatState properly
		CombatState = ECombatState::ECS_Harvesting;
		PlayHarvestingMontage();
		if (HarvestBushEffect)
			UGameplayStatics::SpawnEmitterAtLocation(this, HarvestBushEffect, GetActorLocation());

		if (HarvestBushSound)
			UGameplayStatics::SpawnSoundAtLocation(this, HarvestBushSound, GetActorLocation());
		
		for (const auto& Actor : OutActors)
		{
			HarvestGroundItem(Actor);
		}
	}
}

void ASCCharacter::HarvestGroundItem(AActor* TargetActor)
{
	constexpr float Damage = 15.f;
	constexpr float ServerRate = 1.f;
	// TODO: Create a interface for this so we don't need to cast and our Character class don't need to have a reference
	ASCGroundItem* GroundItem = Cast<ASCGroundItem>(TargetActor);

	// It isn't a Ground Item
	if (GroundItem == nullptr) return;

	GroundItem->ReceiveDamage(Damage);

	FLargeItemInfo* GroundItemInfo = GroundResourcesDataTable->FindRow<FLargeItemInfo>(FName(*UKismetSystemLibrary::GetDisplayName(TargetActor)), TEXT(""));

	if (!GroundItemInfo) return;

	if (GroundItem->GetHealth() > 0.f)
	{
		for (auto& GivenItem : GroundItemInfo->GivenItems)
		{
			const int32 Quantity = FMath::TruncToInt(GivenItem.Quantity * ServerRate * FMath::FRandRange(0.3, 1.2));

			if (Quantity <= 0) continue;
				
			FResourceInfo ResourceInfo;
			ResourceInfo.ResourceID = GivenItem.ResourceID;
			ResourceInfo.Quantity = Quantity;
			ResourceInfo.PreferredToolType = GivenItem.PreferredToolType;
				
			ServerAddHarvestedItem(ResourceInfo);
		}
	}
	else
	{
		const FTransform SpawnTransform = TargetActor->GetActorTransform();
			
		ASCDestructibleHarvestable* SpawnedDestructibleHarvestable = GetWorld()->SpawnActorDeferred<ASCDestructibleHarvestable>(
			GroundItemInfo->DestructibleHarvestableClass,
			SpawnTransform,
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);
			
		TargetActor->Destroy();

		SpawnedDestructibleHarvestable->SetDirectionToFall(GetOwner()->GetActorForwardVector());
		SpawnedDestructibleHarvestable->FinishSpawning(SpawnTransform);
	}
}

void ASCCharacter::PlayHarvestingMontage()
{
	if (!PickupMontage) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UAnimInstance* AnimInstance1P = GetMesh1P()->GetAnimInstance();

	if (AnimInstance) AnimInstance->Montage_Play(PickupMontage);

	if (AnimInstance1P) AnimInstance1P->Montage_Play(PickupMontage);
}

USCItemsContainerComponent* ASCCharacter::GetContainerComponent(const EContainerType ContainerType) const
{
	switch (ContainerType)
	{
	case EContainerType::ECT_PlayerInventory:
		return InventoryComponent;
	case EContainerType::ECT_PlayerHotbar:
		return HotbarComponent;
	case EContainerType::ECT_PlayerStorage:
		break;
	case EContainerType::ECT_PlayerArmor:
		break;
	}

	return nullptr;
}

void ASCCharacter::CraftTimerFinished(USCItemsContainerComponent* ContainerComponent, const FCraftingRecipe& CraftingRecipe)
{
	if (ContainerComponent->ContainRequiredItems(CraftingRecipe.RequiredItems))
	{
		if (ContainerComponent->RemoveItems(CraftingRecipe.RequiredItems))
		{
			// TODO: We'll need to modify here since in the course we have a delay in between, so we can show a progress bar in the future
			// Everything below should be inside a function called "AddCraftedItem"
			const FName ItemIDToCraft = FName(*FString::FromInt(CraftingRecipe.ItemID));

			if (const FItemInformation* ItemInformation = ItemsDataTable->FindRow<FItemInformation>(ItemIDToCraft, TEXT("")))
			{
				ContainerComponent->AddItem(*ItemInformation);

				ClientShowItemAdded(ItemInformation->ItemIcon, ItemInformation->ItemQuantity, ItemInformation->ItemName);
			}
			// Here we should reset the combat state.
		}
	}
	else
	{
		// We never hit here but... we never know. we should reset the combat state here. so we can craft again.
	}
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

		ClientShowItemAdded(ItemInformation->ItemIcon, Resource.Quantity, ItemInformation->ItemName);
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

﻿// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Player/SCPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/SCCharacter.h"
#include "Enums/PlayerStats.h"
#include "Game/SCGameState.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Data/SCItemData.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SCPlayerState.h"
#include "AdvancedSessionsLibrary.h"
#include "BuildingSystem/SCBuildable.h"
#include "Enums/MenuOptionsWidgetType.h"
#include "Game/SCGameMode.h"

void ASCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ASCPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJump);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &ThisClass::ToggleInventory);
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Started, this, &ThisClass::OnLeftMouse);
		EnhancedInputComponent->BindAction(LeftMouseAction, ETriggerEvent::Completed, this, &ThisClass::OnLeftMouseReleased);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
		EnhancedInputComponent->BindAction(DemolishAction, ETriggerEvent::Started, this, &ThisClass::DemolishStructure);
		EnhancedInputComponent->BindAction(DemolishAction, ETriggerEvent::Completed, this, &ThisClass::StopDemolishStructure);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ThisClass::Reload);
		EnhancedInputComponent->BindAction(InviteToTribeAction, ETriggerEvent::Started, this, &ThisClass::InviteToTribe);
		EnhancedInputComponent->BindAction(EnterKeyAction, ETriggerEvent::Started, this, &ThisClass::OnEnterKey);
		EnhancedInputComponent->BindAction(VOIPAction, ETriggerEvent::Started, this, &ThisClass::EnableVOIP);
		EnhancedInputComponent->BindAction(VOIPAction, ETriggerEvent::Completed, this, &ThisClass::DisableVOIP);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASCPlayerController::ClientUpdateItemSlot_Implementation(EContainerType ContainerType, int32 SlotIndex, const FItemInformation& Item)
{
	OnUpdateItemSlotDelegate.Broadcast(ContainerType, SlotIndex, Item);
}

void ASCPlayerController::ClientResetItemSlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	OnResetItemSlotDelegate.Broadcast(ContainerType, SlotIndex);
}

void ASCPlayerController::ClientUpdateArmorSlot_Implementation(EArmorType ArmorType, const FItemInformation& Item)
{
	OnUpdateArmorSlotDelegate.Broadcast(ArmorType, Item);
}

void ASCPlayerController::ClientResetArmorSlot_Implementation(EArmorType ArmorType)
{
	OnResetArmorSlotDelegate.Broadcast(ArmorType);
}

void ASCPlayerController::ClientToggleStorage_Implementation(int32 TotalSlots, ECraftingType StorageType, bool bCloseStorage)
{
	OnToggleInventoryDelegate.Broadcast();
	OnToggleStorageDelegate.Broadcast(TotalSlots, StorageType, bCloseStorage);
}

void ASCPlayerController::ClientUpdateStorageSlots_Implementation(int32 TotalSlots)
{
	OnUpdateStorageSlotsDelegate.Broadcast(TotalSlots);
}

void ASCPlayerController::ClientToggleMenuOptionsWidget_Implementation(EMenuOptionsWidgetType WidgetToShow)
{
	if (const ASCPlayerState* SCPlayerState = Cast<ASCPlayerState>(PlayerState))
	{
		OnToggleMenuOptionsWidgetDelegate.Broadcast(WidgetToShow, SCPlayerState->IsInTribe());
	}
}

void ASCPlayerController::ClientReceiveTribeInvite_Implementation(const FString& TribeID, const FText& TribeName, const FText& SenderName)
{
	OnReceiveTribeInviteDelegate.Broadcast(TribeID, TribeName, SenderName);
}

void ASCPlayerController::ClientReceiveChatMessage_Implementation(const FString& Message, const FText& TribeName, const FText& PlayerName, bool bIsGlobalMessage)
{
	OnReceiveChatMessageDelegate.Broadcast(Message, TribeName, PlayerName, bIsGlobalMessage);
}

void ASCPlayerController::ClientTogglePlayerNameWidget_Implementation(const FText& PlayerName, const FText& TribeName, const bool bIsVisible, const bool bIsFriendly)
{
	OnTogglePlayerNameTagDelegate.Broadcast(PlayerName, TribeName, bIsVisible, bIsFriendly);
}

void ASCPlayerController::ShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName)
{
	OnItemAddedDelegate.Broadcast(ItemIcon, ItemQuantity, ItemName);
}

bool ASCPlayerController::CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	return GetSCCharacter()->CanCraftItem(ItemID, ContainerType, TableType);
}

void ASCPlayerController::CraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType)
{
	return GetSCCharacter()->ServerCraftItem(ItemID, ContainerType, TableType);
}

void ASCPlayerController::UpdatePlayerStats(EPlayerStats PlayerStats, float NewValue)
{
	switch (PlayerStats)
	{
		case EPlayerStats::EPS_Health:
			OnPlayerHealthChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxHealth:
			OnPlayerMaxHealthChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_Food:
			OnPlayerFoodChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxFood:
			OnPlayerMaxFoodChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_Water:
			OnPlayerWaterChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxWater:
			OnPlayerMaxWaterChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_Stamina:
			OnPlayerStaminaChangedDelegate.Broadcast(NewValue);
			break;
		case EPlayerStats::EPS_MaxStamina:
			OnPlayerMaxStaminaChangedDelegate.Broadcast(NewValue);
			break;
	}
}

void ASCPlayerController::UpdatePlayerWindow(UMaterialInstanceDynamic* Material)
{
	OnPlayerWindowInitDelegate.Broadcast(Material);
}

void ASCPlayerController::ClientUpdateDemolishStructureProgress_Implementation(const bool bCancelDemolish, const float DemolishTime)
{
	OnDemolishStructureDelegate.Broadcast(bCancelDemolish, DemolishTime);
}

float ASCPlayerController::GetHealth()
{
	return GetSCCharacter()->GetHealth();
}

float ASCPlayerController::GetMaxHealth()
{
	return GetSCCharacter()->GetMaxHealth();
}

float ASCPlayerController::GetFood()
{
	return GetSCCharacter()->GetFood();
}

float ASCPlayerController::GetMaxFood()
{
	return GetSCCharacter()->GetMaxFood();
}

float ASCPlayerController::GetWater()
{
	return GetSCCharacter()->GetWater();
}

float ASCPlayerController::GetMaxWater()
{
	return GetSCCharacter()->GetMaxWater();
}

float ASCPlayerController::GetStamina()
{
	return GetSCCharacter()->GetStamina();
}

float ASCPlayerController::GetMaxStamina()
{
	return GetSCCharacter()->GetMaxStamina();
}

void ASCPlayerController::RemoveCharacterStorageBoxReference()
{
	GetSCCharacter()->ServerRemoveStorageBoxReference();
}

void ASCPlayerController::RunStorageAction()
{
	return GetSCCharacter()->ServerRunStorageAction();
}

void ASCPlayerController::ShowItemOptionsMenu(int32 Index, EContainerType Container)
{
	OnShowItemOptionsMenuDelegate.Broadcast(Index, Container);
}

void ASCPlayerController::HideItemOptionsMenu()
{
	OnHideItemOptionsMenuDelegate.Broadcast();
}

void ASCPlayerController::DropItem(EContainerType ContainerType, int32 FromIndex)
{
	GetSCCharacter()->ServerDropItem(ContainerType, FromIndex);
}

void ASCPlayerController::SplitItemStack(EContainerType ContainerType, int32 FromIndex)
{
	GetSCCharacter()->ServerSplitItemStack(ContainerType, FromIndex);
}

void ASCPlayerController::ServerSendChatMessage_Implementation(const FString& Message, bool bIsGlobalMessage)
{
	if (const ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		if (ASCGameMode* SCGameMode = Cast<ASCGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			SCGameMode->SendMessageToPlayers(Message, SCPlayerState->GetTribeName(), SCPlayerState->GetPlayerNickname(), bIsGlobalMessage, SCPlayerState->GetTribeID());
		}
	}
}

void ASCPlayerController::ServerCreateTribe_Implementation(const FText& TribeName)
{
	if (ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			FBPUniqueNetId BPUniqueNetId;
			FString UniqueIDString = FString();
					
			UAdvancedSessionsLibrary::GetUniqueNetID(this, BPUniqueNetId);
			UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);
			
			SCPlayerState->SetTribeName(TribeName);
			SCPlayerState->SetTribeID(UniqueIDString);
			SCPlayerState->SetIsInTribe(true);
			SCPlayerState->SetTribeRank(ETribeRank::ETR_Owner);
		
			TArray<FTribeMemberInfo> Members;
			FTribeMemberInfo TribeMemberInfo;
			TribeMemberInfo.PlayerName = SCPlayerState->GetPlayerNickname();
			TribeMemberInfo.bIsOnline = true;
			TribeMemberInfo.PlayerController = this;
			TribeMemberInfo.TribeRank = ETribeRank::ETR_Owner;
			TribeMemberInfo.PlayerIDOffline = UniqueIDString;

			TArray<FTribeLogEntry> Logs;
			const FText LogMessage = FText::FromString(FString::Printf(TEXT("%s created the tribe!"), *SCPlayerState->GetPlayerNickname().ToString()));
			
			Members.Add(TribeMemberInfo);
			Logs.Add(ASCGameState::MakeLogEntry(LogMessage, ETribeLogColor::ETL_Green));
			
			FTribeInfo TribeInfo;
			TribeInfo.Name = TribeName;
			TribeInfo.ID = UniqueIDString;
			TribeInfo.Members = Members;
			TribeInfo.Logs = Logs;
			
			SCGameState->CreateTribe(TribeInfo);
			ClientUpdateTribeInfo(TribeInfo, true);

			MergeTribeStructures(UniqueIDString, TribeName);
		}
	}
}

void ASCPlayerController::ServerJoinTribe_Implementation(const FString& TribeID, const FText& SenderName)
{
	if (ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		if (SCPlayerState->IsInTribe()) return;
		
		if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
		{
			const FTribeInfo* TribeInfo = SCGameState->GetTribeByID(TribeID);

			if (TribeInfo == nullptr) return;

			FBPUniqueNetId BPUniqueNetId;
			FString UniqueIDString = FString();
					
			UAdvancedSessionsLibrary::GetUniqueNetID(this, BPUniqueNetId);
			UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);

			SCPlayerState->SetTribeName(TribeInfo->Name);
			SCPlayerState->SetTribeRank(ETribeRank::ETR_Member);
			SCPlayerState->SetTribeID(TribeID);
			SCPlayerState->SetIsInTribe(true);

			FTribeInfo TribeToUpdate = *TribeInfo;
			
			FTribeMemberInfo TribeMemberInfo;
			TribeMemberInfo.PlayerName = SCPlayerState->GetPlayerNickname();
			TribeMemberInfo.bIsOnline = true;
			TribeMemberInfo.PlayerController = this;
			TribeMemberInfo.TribeRank = ETribeRank::ETR_Member;
			TribeMemberInfo.PlayerIDOffline = UniqueIDString;
			
			const FText LogMessage = FText::FromString(FString::Printf(TEXT("%s Added %s to the tribe"), *SenderName.ToString(), *SCPlayerState->GetPlayerNickname().ToString()));

			TribeToUpdate.Members.Add(TribeMemberInfo);
			TribeToUpdate.Logs.Add(ASCGameState::MakeLogEntry(LogMessage, ETribeLogColor::ETL_Green));

			if (SCGameState->UpdateTribeByID(TribeID, TribeToUpdate))
			{
				ClientUpdateTribeInfo(TribeToUpdate, true);
			}

			MergeTribeStructures(TribeID, TribeInfo->Name);
		}
	}
}

void ASCPlayerController::ServerDemoteTribeMember_Implementation(const FString& TribeMemberID)
{
	if (const ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		const ETribeRank PlayerRank = SCPlayerState->GetTribeRank();
		if (PlayerRank == ETribeRank::ETR_Admin || PlayerRank == ETribeRank::ETR_Owner)
		{
			if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				SCGameState->DemoteTribeMember(SCPlayerState->GetTribeID(), TribeMemberID, SCPlayerState->GetPlayerNickname());
			}
		}
	}
}

void ASCPlayerController::ServerKickTribeMember_Implementation(const FString& TribeMemberID)
{
	if (const ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		const ETribeRank PlayerRank = SCPlayerState->GetTribeRank();
		if (PlayerRank == ETribeRank::ETR_Admin || PlayerRank == ETribeRank::ETR_Owner)
		{
			if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				SCGameState->KickTribeMember(SCPlayerState->GetTribeID(), TribeMemberID, SCPlayerState->GetPlayerNickname());
			}
		}
	}
}

void ASCPlayerController::ServerPromoteTribeMember_Implementation(const FString& TribeMemberID)
{
	if (const ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		const ETribeRank PlayerRank = SCPlayerState->GetTribeRank();
		if (PlayerRank == ETribeRank::ETR_Admin || PlayerRank == ETribeRank::ETR_Owner)
		{
			if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				SCGameState->PromoteTribeMember(SCPlayerState->GetTribeID(), TribeMemberID, SCPlayerState->GetPlayerNickname());
			}
		}
	}
}

void ASCPlayerController::ServerLeaveTribe_Implementation()
{
	if (const ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		const ETribeRank PlayerRank = SCPlayerState->GetTribeRank();
		if (PlayerRank == ETribeRank::ETR_Admin || PlayerRank == ETribeRank::ETR_Member)
		{
			if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				FBPUniqueNetId BPUniqueNetId;
				FString UniqueIDString = FString();
					
				UAdvancedSessionsLibrary::GetUniqueNetID(this, BPUniqueNetId);
				UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);
				
				SCGameState->LeaveTribe(SCPlayerState->GetTribeID(), UniqueIDString);
			}
		}
	}
}

void ASCPlayerController::ServerSetTribeMessage_Implementation(const FText& Message)
{
	if (const ASCPlayerState* SCPlayerState = GetPlayerState<ASCPlayerState>())
	{
		if (!SCPlayerState->IsInTribe()) return;

		const ETribeRank PlayerRank = SCPlayerState->GetTribeRank();
		if (PlayerRank == ETribeRank::ETR_Owner || PlayerRank == ETribeRank::ETR_Admin)
		{
			if (ASCGameState* SCGameState = Cast<ASCGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				SCGameState->SetTribeMessage(SCPlayerState->GetTribeID(), Message, SCPlayerState->GetPlayerNickname());
			}
		}
	}
}

void ASCPlayerController::ClientUpdateTribeInfo_Implementation(const FTribeInfo& TribeInfo, const bool bForceToggleTribeMenu)
{
	OnUpdateTribeDelegate.Broadcast(TribeInfo);
	if (bForceToggleTribeMenu)
	{
		OnToggleMenuOptionsWidgetDelegate.Broadcast(EMenuOptionsWidgetType::EMO_TribesWidget, true);
	}
}

void ASCPlayerController::ServerSpendSkillPoint_Implementation(EPlayerStats StatToUpgrade)
{
	IPlayerInterface::Execute_SpendSkillPoint(GetSCCharacter(), StatToUpgrade);
}

void ASCPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), MovementVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), MovementVector.X);
	}
}

void ASCPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASCPlayerController::Jump()
{
	GetCharacter()->Jump();
}

void ASCPlayerController::StopJump()
{
	GetCharacter()->StopJumping();
}

void ASCPlayerController::ToggleInventory()
{
	OnToggleInventoryDelegate.Broadcast();
}

void ASCPlayerController::OnLeftMouse()
{
	// TODO: Create a interface for the Character so I don't need to cast.
	GetSCCharacter()->SetLeftButtonPressed(true);
	GetSCCharacter()->UseEquipable();
}

void ASCPlayerController::OnLeftMouseReleased()
{
	GetSCCharacter()->SetLeftButtonPressed(false);
}

void ASCPlayerController::Interact()
{
	GetSCCharacter()->Interact();
}

void ASCPlayerController::Sprint()
{
	ASCCharacter* Char = GetSCCharacter();

	// TODO: Need to refactor this so we can have a sprint with prediction
	Char->GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	Char->ServerSprint(true);
}

void ASCPlayerController::StopSprint()
{
	ASCCharacter* Char = GetSCCharacter();

	Char->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	Char->ServerSprint(false);
}

void ASCPlayerController::DemolishStructure()
{
	GetSCCharacter()->StartDemolish();
}

void ASCPlayerController::StopDemolishStructure()
{
	GetSCCharacter()->StopDemolish();
}

void ASCPlayerController::Reload()
{
	GetSCCharacter()->ServerReload();
}

void ASCPlayerController::InviteToTribe()
{
	GetSCCharacter()->InviteToTribe();
}

void ASCPlayerController::MergeTribeStructures(const FString& TribeID, const FText& OwnerName)
{
	TArray<AActor*> OutActors;
	// TODO: Very expensive function to run, we need to improve this by probably holding the reference of all buildings we put in the world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCBuildable::StaticClass(), OutActors);

	if (OutActors.Num() <= 0) return;

	FBPUniqueNetId BPUniqueNetId;
	FString UniqueIDString = FString();
					
	UAdvancedSessionsLibrary::GetUniqueNetID(this, BPUniqueNetId);
	UAdvancedSessionsLibrary::UniqueNetIdToString(BPUniqueNetId, UniqueIDString);

	TArray<ASCBuildable*> OwningBuildables;
	for (AActor* OutActor : OutActors)
	{
		if (ASCBuildable* Buildable = Cast<ASCBuildable>(OutActor))
		{
			if (!Buildable->GetTribeID().IsEmpty()) continue;

			if (!UniqueIDString.IsEmpty() && !Buildable->GetOwnerNetID().IsEmpty() && UniqueIDString.Equals(Buildable->GetOwnerNetID()))
			{
				OwningBuildables.Add(Buildable);
			}
		}
	}

	if (OwningBuildables.Num() <= 0) return;

	for (ASCBuildable* Buildable : OwningBuildables)
	{
		Buildable->SetTribeID(TribeID);
		Buildable->SetOwnerName(OwnerName);
	}
}

void ASCPlayerController::OnEnterKey()
{
	OnEnterKeyPressedDelegate.Broadcast();
}

void ASCPlayerController::EnableVOIP()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, FString::Printf(TEXT("ToggleSpeaking 1")));
	OnToggleVOIPDelegate.Broadcast(true);
}

void ASCPlayerController::DisableVOIP()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, FString::Printf(TEXT("ToggleSpeaking 0")));
	OnToggleVOIPDelegate.Broadcast(false);
}

ASCCharacter* ASCPlayerController::GetSCCharacter()
{
	return SCCharacter = SCCharacter == nullptr ? Cast<ASCCharacter>(GetCharacter()) : SCCharacter;
}

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Crafting/Data/CraftingData.h"
#include "Enums/ContainerType.h"
#include "GameFramework/PlayerController.h"
#include "Tribe/SCTribeData.h"
#include "SCPlayerController.generated.h"

enum class EMenuOptionsWidgetType : uint8;
enum class EArmorType : uint8;
enum class EPlayerStats : uint8;
class ASCCharacter;
struct FItemInformation;
DECLARE_MULTICAST_DELEGATE(FOnToggleInventorySignature)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemSlotSignature, EContainerType/*ContainerType*/, int32/*SlotIndex*/, const FItemInformation&/*Item*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnResetItemSlotSignature, EContainerType/*ContainerType*/, int32/*SlotIndex*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedSignature, UTexture2D*/*ItemIcon*/, int32/*ItemQuantity*/, FText/*ItemName*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsChangedSignature, float/*NewValue*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateArmorSlotSignature, EArmorType/*ArmorType*/, const FItemInformation&/*Item*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnResetArmorSlotSignature, EArmorType/*ArmorType*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerWindowInitSignature, UMaterialInstanceDynamic*/*Material*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDemolishStructureSignature, bool/*bCancelDemolish*/, float/*DemolishTime*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnToggleStorageSignature, int32/*TotalSlots*/, ECraftingType/*StorageType*/, bool /*bCloseStorage*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateStorageSlotsSignature, int32/*TotalSlots*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnShowItemOptionsMenuSignature, int32/*Index*/, EContainerType/*Container*/);
DECLARE_MULTICAST_DELEGATE(FOnHideItemOptionsMenuSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnToggleMenuOptionsWidgetSignature, EMenuOptionsWidgetType/*WidgetToShow*/, bool/*bIsInTribe*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateTribeSignature, const FTribeInfo&/*TribeInfo*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnReceiveTribeInviteSignature, const FString&/*TribeID*/, const FText&/*TribeName*/, const FText&/*SenderName*/);

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SURVIVALCRAFT_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void ClientUpdateItemSlot(EContainerType ContainerType, int32 SlotIndex, const FItemInformation& Item);
	
	UFUNCTION(Client, Reliable)
	void ClientResetItemSlot(EContainerType ContainerType, int32 SlotIndex);

	UFUNCTION(Client, Reliable)
	void ClientUpdateArmorSlot(EArmorType ArmorType, const FItemInformation& Item);

	UFUNCTION(Client, Reliable)
	void ClientResetArmorSlot(EArmorType ArmorType);

	UFUNCTION(Client, Reliable)
	void ClientUpdateDemolishStructureProgress(const bool bCancelDemolish, const float DemolishTime);

	UFUNCTION(Client, Reliable)
	void ClientToggleStorage(int32 TotalSlots, ECraftingType StorageType, bool bCloseStorage = false);

	UFUNCTION(Client, Reliable)
	void ClientUpdateStorageSlots(int32 TotalSlots);

	UFUNCTION(Client, Unreliable)
	void ClientToggleMenuOptionsWidget(EMenuOptionsWidgetType WidgetToShow);

	UFUNCTION(Client, Reliable)
	void ClientUpdateTribeInfo(const FTribeInfo& TribeInfo, const bool bForceToggleTribeMenu = false);

	UFUNCTION(Client, Reliable)
	void ClientReceiveTribeInvite(const FString& TribeID, const FText& TribeName, const FText& SenderName);
	
	void ShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName);
	bool CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);
	void CraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);
	void UpdatePlayerStats(EPlayerStats PlayerStats, float NewValue);
	void UpdatePlayerWindow(UMaterialInstanceDynamic* Material);
	float GetHealth();
	float GetMaxHealth();
	float GetFood();
	float GetMaxFood();
	float GetWater();
	float GetMaxWater();
	float GetStamina();
	float GetMaxStamina();
	
	void RemoveCharacterStorageBoxReference();
	void RunStorageAction();
	void ShowItemOptionsMenu(int32 Index, EContainerType Container);
	void HideItemOptionsMenu();
	void DropItem(EContainerType ContainerType, int32 FromIndex);
	void SplitItemStack(EContainerType ContainerType, int32 FromIndex);

	UFUNCTION(Server, Reliable)
	void ServerSpendSkillPoint(EPlayerStats StatToUpgrade);

	UFUNCTION(Server, Reliable)
	void ServerCreateTribe(const FText& TribeName);

	UFUNCTION(Server, Reliable)
	void ServerJoinTribe(const FString& TribeID, const FText& SenderName);

	UFUNCTION(Server, Reliable)
	void ServerDemoteTribeMember(const FString& TribeMemberID);

	UFUNCTION(Server, Reliable)
	void ServerKickTribeMember(const FString& TribeMemberID);

	UFUNCTION(Server, Reliable)
	void ServerPromoteTribeMember(const FString& TribeMemberID);

	UFUNCTION(Server, Reliable)
	void ServerLeaveTribe();
	
	FOnToggleInventorySignature OnToggleInventoryDelegate;
	FOnUpdateItemSlotSignature OnUpdateItemSlotDelegate;
	FOnResetItemSlotSignature OnResetItemSlotDelegate;
	FOnItemAddedSignature OnItemAddedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerHealthChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerMaxHealthChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerFoodChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerMaxFoodChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerWaterChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerMaxWaterChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerStaminaChangedDelegate;
	FOnPlayerStatsChangedSignature OnPlayerMaxStaminaChangedDelegate;
	FOnUpdateArmorSlotSignature OnUpdateArmorSlotDelegate;
	FOnResetArmorSlotSignature OnResetArmorSlotDelegate;
	FOnPlayerWindowInitSignature OnPlayerWindowInitDelegate;
	FOnDemolishStructureSignature OnDemolishStructureDelegate;
	FOnToggleStorageSignature OnToggleStorageDelegate;
	FOnUpdateStorageSlotsSignature OnUpdateStorageSlotsDelegate;
	FOnShowItemOptionsMenuSignature OnShowItemOptionsMenuDelegate;
	FOnHideItemOptionsMenuSignature OnHideItemOptionsMenuDelegate;
	FOnToggleMenuOptionsWidgetSignature OnToggleMenuOptionsWidgetDelegate;
	FOnUpdateTribeSignature OnUpdateTribeDelegate;
	FOnReceiveTribeInviteSignature OnReceiveTribeInviteDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	ASCCharacter* GetSCCharacter();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	void ToggleInventory();
	void OnLeftMouse();
	void OnLeftMouseReleased();
	void Interact();
	void Sprint();
	void StopSprint();
	void DemolishStructure();
	void StopDemolishStructure();
	void Reload();
	void InviteToTribe();

	UPROPERTY()
	ASCCharacter* SCCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftMouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DemolishAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InviteToTribeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> InputMappingContext;
};

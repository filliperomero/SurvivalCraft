// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Crafting/Data/CraftingData.h"
#include "Enums/CombatState.h"
#include "Enums/PlayerStats.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Items/Data/EquipableData.h"
#include "Logging/LogMacros.h"
#include "SCCharacter.generated.h"

class UVOIPTalker;
class USphereComponent;
class UWidgetComponent;
class ASCStorage;
class ASCBuildable;
class USCBuildingComponent;
class USCItemsContainerComponent;
struct FResourceInfo;
class ASCEquipableItem;
class USCPlayerHotbarComponent;
class USCPlayerInventoryComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogSCCharacter, Log, All);

UCLASS(config=Game)
class SURVIVALCRAFT_API ASCCharacter : public ACharacter, public IPlayerInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ASCCharacter();

	/** Player Interface */
	virtual ASCPlayerController* GetSCPlayerController_Implementation() override;
	virtual void OnSlotDrop_Implementation(EContainerType TargetContainerType, EContainerType FromContainerType, int32 FromIndex, int32 ToIndex, EArmorType ArmorType) override;
	virtual void OnEquipArmor_Implementation(EContainerType FromContainerType, int32 FromIndex, EArmorType ArmorType) override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void SpendSkillPoint_Implementation(EPlayerStats StatToUpgrade) override;
	virtual bool HasArmorInSlot_Implementation(EPhysicalSurface PhysicalSurface) override;
	/** Player Interface */

	/** Combat Interface */
	virtual int32 GetArmorAmount_Implementation() override;
	/** Combat Interface */

	UFUNCTION(BlueprintCallable)
	void UseHotBar(const int32 Index);

	UFUNCTION()
	void UseEquipable();

	void Interact();
	bool CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);
	void StartDemolish();
	void StopDemolish();
	void SetLeftButtonPressed(const bool bPressed);
	void InviteToTribe();
	
	UFUNCTION(BlueprintCallable)
	void FinishEquipable();
	
	UFUNCTION(BlueprintCallable)
	void FinishHarvesting();
	
	UFUNCTION(BlueprintCallable)
	void EquipableHit();

	void PlayEquipableMontage(FName SectionName);

	UFUNCTION(Server, Reliable)
	void ServerAddHarvestedItem(const FResourceInfo& Resource);

	UFUNCTION(Server, Reliable)
	void ServerUnequipCurrentItem(int32 Index);

	UFUNCTION(Server, Reliable)
	void ServerOnSlotDrop(EContainerType TargetContainerType, EContainerType FromContainerType, int32 FromIndex, int32 ToIndex, EArmorType ArmorType);

	UFUNCTION(Server, Reliable)
	void ServerDropItem(EContainerType TargetContainerType, int32 FromIndex);

	UFUNCTION(Server, Reliable)
	void ServerSplitItemStack(EContainerType TargetContainerType, int32 FromIndex);

	UFUNCTION(Server, Reliable)
	void ServerOnEquipArmor(EContainerType FromContainerType, int32 FromIndex, EArmorType ArmorType);

	UFUNCTION(Server, Reliable)
	void ServerOnUnequipArmor(EArmorType ArmorType);

	UFUNCTION(Server, Reliable)
	void ServerCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);

	UFUNCTION(Server, Reliable)
	void ServerSprint(bool bInIsSprinting);

	// TODO: Check if we really need to have this function here or we can call it directly from Buildable class
	UFUNCTION(Client, Reliable)
	void ClientToggleBuildableInfoWidget(ASCBuildable* Buildable, ESlateVisibility WidgetVisibility, bool bShowInteractText, bool bShowOptionsText, const FText& StructureName, const FText& OwnerName, float InCurrentHealth, float InMaxHealth);

	UFUNCTION(Client, Reliable)
	void ClientShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, const FText& ItemName);

	UFUNCTION(Server, Reliable)
	void ServerRemoveStorageBoxReference();

	UFUNCTION(Server, Reliable)
	void ServerRunStorageAction();

	UFUNCTION(Server, Reliable)
	void ServerReload();

	UFUNCTION(Server, Reliable)
	void ServerAddToItemDurability(int32 Amount);
	
	UPROPERTY()
	TObjectPtr<ASCStorage> StorageBox;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	UFUNCTION()
	void OnPlayerNameSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerNameSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UWidgetComponent> PlayerNameWidgetComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> PlayerNameSphereComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> PlayerArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneCaptureComponent2D> PlayerWindow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterial> PlayerWindowMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USCPlayerInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USCPlayerHotbarComponent> HotbarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USCBuildingComponent> BuildingComponent;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	EEquipableState EquipableState = EEquipableState::EES_Default;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> EquipableMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> PickupMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> HarvestBushEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> HarvestBushSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> GroundResourcesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> PlayerCraftingRecipesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> CraftingBenchRecipesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> AdvanceCraftingBenchRecipesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ConsumablesDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VOIP", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundAttenuation> VOIPAttenuationSettings;

	UPROPERTY(Replicated)
	TObjectPtr<ASCEquipableItem> EquippedItem;

	UPROPERTY(ReplicatedUsing = OnRep_HelmetSlot)
	TObjectPtr<ASCItemMaster> HelmetSlot;

	UFUNCTION()
	void OnRep_HelmetSlot();

	UPROPERTY(ReplicatedUsing = OnRep_ChestSlot)
	TObjectPtr<ASCItemMaster> ChestSlot;

	UFUNCTION()
	void OnRep_ChestSlot();

	UPROPERTY(ReplicatedUsing = OnRep_PantsSlot)
	TObjectPtr<ASCItemMaster> PantsSlot;

	UFUNCTION()
	void OnRep_PantsSlot();

	UPROPERTY(ReplicatedUsing = OnRep_BootsSlot)
	TObjectPtr<ASCItemMaster> BootsSlot;

	UFUNCTION()
	void OnRep_BootsSlot();

	int32 EquippedItemIndex = -1;

	UPROPERTY()
	TObjectPtr<ASCEquipableItem> FP_EquippedItem;

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	bool bDead = false;
	bool bCanSprint = true;
	bool bIsSprinting = false;

	/* Player Stats */
	void UpdatePlayerStatsUI(EPlayerStats PlayerStats, float NewValue);
	void AddToPlayerStats(EPlayerStats PlayerStats, float NewValue);
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	UFUNCTION()
	void OnRep_MaxHealth(float LastMaxHealth);

	bool bStarving = false;

	UPROPERTY(EditAnywhere, Category = "Player Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float FoodDecreasePercentage = 2;
	
	UPROPERTY(ReplicatedUsing = OnRep_Food, VisibleAnywhere, Category = "Player Stats")
	float Food = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxFood, EditAnywhere, Category = "Player Stats")
	float MaxFood = 100.f;

	UFUNCTION()
	void OnRep_Food(float LastFood);

	UFUNCTION()
	void OnRep_MaxFood(float LastMaxFood);

	bool bDehydrated = false;
	
	UPROPERTY(EditAnywhere, Category = "Player Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float WaterDecreasePercentage = 4;

	UPROPERTY(ReplicatedUsing = OnRep_Water, VisibleAnywhere, Category = "Player Stats")
	float Water = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxWater, EditAnywhere, Category = "Player Stats")
	float MaxWater = 100.f;

	UFUNCTION()
	void OnRep_Water(float LastWater);

	UFUNCTION()
	void OnRep_MaxWater(float LastMaxWater);

	// Variable used to update the UI
	int32 LastStamina = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Stamina, VisibleAnywhere, Category = "Player Stats")
	float Stamina = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_MaxStamina, EditAnywhere, Category = "Player Stats")
	float MaxStamina = 100.f;

	UFUNCTION()
	void OnRep_Stamina(float InLastStamina);

	UFUNCTION()
	void OnRep_MaxStamina(float LastMaxStamina);

	FTimerHandle StatDrainTimer;
	FTimerHandle HealthDecreaseTimer;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float StatDrainDelay = 60.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float HealthDecreaseDelay = 15.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float HealthDecreasePercentage = 4.f;

	UFUNCTION()
	void PassiveStatDrain();

	UFUNCTION()
	void DecreaseHealthOverTime();

	float HealthOvertimeAmount = 0;
	FTimerDelegate HealthOvertimeDelegate;
	FTimerHandle HealthOvertimeTimer;
	float FoodOvertimeAmount = 0;
	FTimerDelegate FoodOvertimeDelegate;
	FTimerHandle FoodOvertimeTimer;
	float WaterOvertimeAmount = 0;
	FTimerDelegate WaterOvertimeDelegate;
	FTimerHandle WaterOvertimeTimer;

	UFUNCTION()
	void OvertimePlayerStatsFinished(EPlayerStats PlayerStats);
	/* End of Player Stats */

	UFUNCTION()
	void OnRep_CombatState();

	UFUNCTION(Server, Reliable)
	void ServerUseHotBar(const int32 Index);
	
	void SpawnEquipable(TSubclassOf<AActor> EquipableItemClass, FItemInformation ItemInformation, int32 EquippedIndex);

	// UFUNCTION(NetMulticast, Reliable)
	// void MulticastEquipItem(AActor* Target, FName SocketName, EEquipableState InEquippedState);

	UFUNCTION(Client, Reliable)
	void ClientSpawnEquipable(TSubclassOf<ASCEquipableItem> EquipableItemClass, FName SocketName);

	UFUNCTION(Client, Reliable)
	void ClientUnequipEquipable();

	UFUNCTION(Server, Reliable)
	void ServerUseEquipable(FRotator ClientCameraRotation);

	UFUNCTION(Server, Reliable)
	void ServerReleaseLeftButton(FRotator ClientCameraRotation);

	UFUNCTION(Server, Reliable)
	void ServerInteract(FRotator ClientCameraRotation);

	UFUNCTION(Server, Reliable)
	void ServerLineTraceStorageBox(FRotator ClientCameraRotation);

	bool LineTraceFunction(FRotator ClientCameraRotation, AActor*& HitActor, UClass* InterfaceToCheck);
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEquipableMontage(FName SectionName);

	UFUNCTION(Server, Reliable)
	void ServerStartDemolishTimer(FRotator ClientCameraRotation);

	UFUNCTION(Server, Reliable)
	void ServerStopDemolishTimer();

	UFUNCTION(Server, Reliable)
	void ServerInviteToTribe(FRotator ClientCameraRotation);
	
	float DemolishTime = 5.f;
	FTimerDelegate DemolishTimerDelegate;
	FTimerHandle DemolishTimerHandle;

	UFUNCTION()
	void DemolishTimerFinished(FRotator ClientCameraRotation);

	void HarvestGroundItem(AActor* TargetActor);
	void PlayHarvestingMontage();
	USCItemsContainerComponent* GetContainerComponent(const EContainerType ContainerType) const;
	ASCItemMaster* GetArmorSlot(EArmorType ArmorType);
	void ClearArmorSlot(EArmorType ArmorType);
	void InitializePlayerWindow();
	void DamageArmorSlot(const float Damage, ASCItemMaster* ArmorSlot, EArmorType ArmorType);
	void PlaceBuildable();

	UPROPERTY()
	TObjectPtr<UVOIPTalker> VOIPTalker;
	
	void InitializeVOIP();

	FTimerDelegate CraftTimerDelegate;
	FTimerHandle CraftTimerHandle;

	UFUNCTION()
	void CraftTimerFinished(USCItemsContainerComponent* ContainerComponent, const FCraftingRecipe& CraftingRecipe);

	bool bCanUseEquipable = true;

	int32 StructureIDToBuild = -1;

	bool bLeftButtonPressed = false;
	FTimerHandle EquipableTimer;

	UFUNCTION()
	void EquipableTimerFinished();

public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return Camera; }
	FORCEINLINE ASCEquipableItem* GetEquippedItem() const { return EquippedItem; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetFood() const { return Food; }
	FORCEINLINE float GetMaxFood() const { return MaxFood; }
	FORCEINLINE float GetWater() const { return Water; }
	FORCEINLINE float GetMaxWater() const { return MaxWater; }
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE USCBuildingComponent* GetBuildingComponent() const { return BuildingComponent; }
	FORCEINLINE USCPlayerHotbarComponent* GetHotbarComponent() const { return HotbarComponent; }
	FORCEINLINE USCPlayerInventoryComponent* GetPlayerInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE int32 GetStructureIDToBuild() const { return StructureIDToBuild; }
	FORCEINLINE void ResetStructureIDToBuild() { StructureIDToBuild = -1; }
	FORCEINLINE UArrowComponent* GetPlayerArrow() const { return PlayerArrow; }
	FORCEINLINE int32 GetEquippedItemIndex() const { return EquippedItemIndex; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE void SetCombatState(ECombatState InCombatState) { CombatState = InCombatState; }
	FORCEINLINE ASCEquipableItem* GetFPEquippedItem() const { return FP_EquippedItem;}
};

// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Crafting/Data/CraftingData.h"
#include "Enums/CombatState.h"
#include "Enums/PlayerStats.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerInterface.h"
#include "Items/Data/EquipableData.h"
#include "Logging/LogMacros.h"
#include "SCCharacter.generated.h"

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
class SURVIVALCRAFT_API ASCCharacter : public ACharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ASCCharacter();

	/** Player Interface */
	virtual ASCPlayerController* GetSCPlayerController_Implementation() override;
	virtual void OnSlotDrop_Implementation(EContainerType TargetContainerType, EContainerType FromContainerType, int32 FromIndex, int32 ToIndex, EArmorType ArmorType) override;
	/** Player Interface */

	UFUNCTION(BlueprintCallable)
	void UseHotBar(const int32 Index);

	UFUNCTION()
	void UseEquipable();

	void Interact();
	bool CanCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);
	
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
	void ServerCraftItem(const int32 ItemID, const EContainerType ContainerType, const ECraftingType TableType);

	UFUNCTION(Server, Reliable)
	void ServerSprint(bool bInIsSprinting);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

private:
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	TObjectPtr<USkeletalMeshComponent> Mesh1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UArrowComponent> PlayerArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USCPlayerInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USCPlayerHotbarComponent> HotbarComponent;

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
	TObjectPtr<UDataTable> ConsumablesDataTable;

	UPROPERTY()
	TObjectPtr<ASCEquipableItem> EquippedItem;

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
	void UpdatePlayerStats(EPlayerStats PlayerStats, float NewValue);
	
	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	bool bStarving = false;

	UPROPERTY(EditAnywhere, Category = "Player Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float FoodDecreasePercentage = 2;
	
	UPROPERTY(ReplicatedUsing = OnRep_Food, VisibleAnywhere, Category = "Player Stats")
	float Food = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxFood = 100.f;

	UFUNCTION()
	void OnRep_Food(float LastFood);

	bool bDehydrated = false;
	
	UPROPERTY(EditAnywhere, Category = "Player Stats", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float WaterDecreasePercentage = 4;

	UPROPERTY(ReplicatedUsing = OnRep_Water, VisibleAnywhere, Category = "Player Stats")
	float Water = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxWater = 100.f;

	UFUNCTION()
	void OnRep_Water(float LastWater);

	// Variable used to update the UI
	int32 LastStamina = 0;

	UPROPERTY(ReplicatedUsing = OnRep_Stamina, VisibleAnywhere, Category = "Player Stats")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxStamina = 100.f;

	UFUNCTION()
	void OnRep_Stamina(float InLastStamina);

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, Category = "Player Stats")
	int32 SkillPoints = 0;

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
	void ServerUseEquipable();

	UFUNCTION(Server, Reliable)
	void ServerInteract();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEquipableMontage(FName SectionName);

	UFUNCTION(Client, Reliable)
	void ClientShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, const FText& ItemName);

	void HarvestGroundItem(AActor* TargetActor);
	void PlayHarvestingMontage();
	USCItemsContainerComponent* GetContainerComponent(const EContainerType ContainerType) const;

	FTimerDelegate CraftTimerDelegate;
	FTimerHandle CraftTimerHandle;

	UFUNCTION()
	void CraftTimerFinished(USCItemsContainerComponent* ContainerComponent, const FCraftingRecipe& CraftingRecipe);

	bool bCanUseEquipable = true;

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
};

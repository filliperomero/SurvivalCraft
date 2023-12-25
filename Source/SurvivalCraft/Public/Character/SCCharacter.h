﻿// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/PlayerInterface.h"
#include "Items/Data/EquipableData.h"
#include "Logging/LogMacros.h"
#include "SCCharacter.generated.h"

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
	virtual void OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType FromContainer, int32 FromIndex, int32 ToIndex, EArmorType ArmorType) override;
	/** Player Interface */

	UFUNCTION(BlueprintCallable)
	void UseHotBar(const int32 Index);

	UFUNCTION()
	void UseEquipable();
	
	UFUNCTION(BlueprintCallable)
	void FinishEquipable();
	
	UFUNCTION(BlueprintCallable)
	void EquipableHit();

	void PlayEquipableMontage(FName SectionName);

	UFUNCTION(Server, Reliable)
	void ServerAddHarvestedItem(const FResourceInfo& Resource);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resources", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ItemsDataTable;

	UPROPERTY()
	TObjectPtr<ASCEquipableItem> EquippedItem;

	UPROPERTY()
	TObjectPtr<ASCEquipableItem> FP_EquippedItem;

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
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayEquipableMontage(FName SectionName);

	bool bCanUseEquipable = true;

public:
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return Camera; }
	FORCEINLINE ASCEquipableItem* GetEquippedItem() const { return EquippedItem; }
};

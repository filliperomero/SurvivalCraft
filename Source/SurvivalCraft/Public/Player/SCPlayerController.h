// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Enums/ContainerType.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

class ASCCharacter;
struct FItemInformation;
DECLARE_MULTICAST_DELEGATE(FOnToggleInventorySignature)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemSlotSignature, EContainerType/*ContainerType*/, int32/*SlotIndex*/, const FItemInformation&/*Item*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnResetItemSlotSignature, EContainerType/*ContainerType*/, int32/*SlotIndex*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemAddedSignature, UTexture2D*/*ItemIcon*/, int32/*ItemQuantity*/, FText/*ItemName*/);

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SURVIVALCRAFT_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void UpdateItemSlot(EContainerType ContainerType, int32 SlotIndex, const FItemInformation& Item);
	void ResetItemSlot(EContainerType ContainerType, int32 SlotIndex);
	void ShowItemAdded(UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName);
	
	FOnToggleInventorySignature OnToggleInventoryDelegate;
	FOnUpdateItemSlotSignature OnUpdateItemSlotDelegate;
	FOnResetItemSlotSignature OnResetItemSlotDelegate;
	FOnItemAddedSignature OnItemAddedDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJump();
	void ToggleInventory();
	void OnLeftMouse();

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
	TObjectPtr<UInputMappingContext> InputMappingContext;
};

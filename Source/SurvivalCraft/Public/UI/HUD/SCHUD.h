// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCHUD.generated.h"

class USCChatWidgetController;
class USCTribeWidgetController;
class USCPlayerStatsMenuWidgetController;
class USCCraftingMenuWidgetController;
class USCHotbarMenuWidgetController;
class USCInventoryMenuWidgetController;
struct FWidgetControllerParams;
class USCOverlayWidgetController;
class USCUserWidget;

UCLASS()
class SURVIVALCRAFT_API ASCHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(APlayerController* PC, APlayerState* PS);

	USCOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	USCInventoryMenuWidgetController* GetInventoryMenuWidgetController(const FWidgetControllerParams& WCParams);
	USCHotbarMenuWidgetController* GetHotbarMenuWidgetController(const FWidgetControllerParams& WCParams);
	USCCraftingMenuWidgetController* GetCraftingMenuWidgetController(const FWidgetControllerParams& WCParams);
	USCPlayerStatsMenuWidgetController* GetPlayerStatsMenuWidgetController(const FWidgetControllerParams& WCParams);
	USCTribeWidgetController* GetTribeWidgetController(const FWidgetControllerParams& WCParams);
	USCChatWidgetController* GetChatWidgetController(const FWidgetControllerParams& WCParams);

private:
	UPROPERTY()
	TObjectPtr<USCUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USCUserWidget> OverlayWidgetClass;

	/** Widget Controller's */
	UPROPERTY()
	TObjectPtr<USCOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USCInventoryMenuWidgetController> InventoryMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCInventoryMenuWidgetController> InventoryMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USCHotbarMenuWidgetController> HotbarMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCHotbarMenuWidgetController> HotbarMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USCCraftingMenuWidgetController> CraftingMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCCraftingMenuWidgetController> CraftingMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USCPlayerStatsMenuWidgetController> PlayerStatsMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCPlayerStatsMenuWidgetController> PlayerStatsMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USCTribeWidgetController> TribeWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCTribeWidgetController> TribeWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USCChatWidgetController> ChatWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USCChatWidgetController> ChatWidgetControllerClass;
};

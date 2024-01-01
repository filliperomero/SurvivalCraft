// Copyright Fillipe Romero

#include "UI/HUD/SCHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/SCUserWidget.h"
#include "UI/WidgetController/SCCraftingMenuWidgetController.h"
#include "UI/WidgetController/SCHotbarMenuWidgetController.h"
#include "UI/WidgetController/SCInventoryMenuWidgetController.h"
#include "UI/WidgetController/SCOverlayWidgetController.h"
#include "UI/WidgetController/SCPlayerStatsMenuWidgetController.h"
#include "UI/WidgetController/SCWidgetController.h"

void ASCHUD::InitOverlay(APlayerController* PC, APlayerState* PS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_SCHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_SCHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<USCUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS);
	USCOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);

	// Broadcast all Initial values
	WidgetController->BroadcastInitialValues();

	Widget->AddToViewport();
}

USCOverlayWidgetController* ASCHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<USCOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

USCInventoryMenuWidgetController* ASCHUD::GetInventoryMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (InventoryMenuWidgetController == nullptr)
	{
		InventoryMenuWidgetController = NewObject<USCInventoryMenuWidgetController>(this, InventoryMenuWidgetControllerClass);
		InventoryMenuWidgetController->SetWidgetControllerParams(WCParams);
		InventoryMenuWidgetController->BindCallbacksToDependencies();
	}

	return InventoryMenuWidgetController;
}

USCHotbarMenuWidgetController* ASCHUD::GetHotbarMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (HotbarMenuWidgetController == nullptr)
	{
		HotbarMenuWidgetController = NewObject<USCHotbarMenuWidgetController>(this, HotbarMenuWidgetControllerClass);
		HotbarMenuWidgetController->SetWidgetControllerParams(WCParams);
		HotbarMenuWidgetController->BindCallbacksToDependencies();
	}

	return HotbarMenuWidgetController;
}

USCCraftingMenuWidgetController* ASCHUD::GetCraftingMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (CraftingMenuWidgetController == nullptr)
	{
		CraftingMenuWidgetController = NewObject<USCCraftingMenuWidgetController>(this, CraftingMenuWidgetControllerClass);
		CraftingMenuWidgetController->SetWidgetControllerParams(WCParams);
		CraftingMenuWidgetController->BindCallbacksToDependencies();
	}

	return CraftingMenuWidgetController;
}

USCPlayerStatsMenuWidgetController* ASCHUD::GetPlayerStatsMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (PlayerStatsMenuWidgetController == nullptr)
	{
		PlayerStatsMenuWidgetController = NewObject<USCPlayerStatsMenuWidgetController>(this, PlayerStatsMenuWidgetControllerClass);
		PlayerStatsMenuWidgetController->SetWidgetControllerParams(WCParams);
		PlayerStatsMenuWidgetController->BindCallbacksToDependencies();
	}

	return PlayerStatsMenuWidgetController;
}

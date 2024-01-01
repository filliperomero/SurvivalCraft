// Copyright Fillipe Romero

#include "SCBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SCHUD.h"
#include "UI/WidgetController/SCWidgetController.h"

bool USCBlueprintFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ASCHUD*& OutSCHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutSCHUD = Cast<ASCHUD>(PC->GetHUD());
		if(OutSCHUD)
		{
			OutWCParams.PlayerState = PC->PlayerState;
			OutWCParams.PlayerController = PC;
			
			return true;
		}
	}

	return false;
}

USCOverlayWidgetController* USCBlueprintFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetOverlayWidgetController(WCParams);
	}
	
	return nullptr;
}

USCInventoryMenuWidgetController* USCBlueprintFunctionLibrary::GetInventoryMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetInventoryMenuWidgetController(WCParams);
	}
	
	return nullptr;
}

USCHotbarMenuWidgetController* USCBlueprintFunctionLibrary::GetHotbarMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetHotbarMenuWidgetController(WCParams);
	}
	
	return nullptr;
}

USCCraftingMenuWidgetController* USCBlueprintFunctionLibrary::GetCraftingMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetCraftingMenuWidgetController(WCParams);
	}
	
	return nullptr;
}

USCPlayerStatsMenuWidgetController* USCBlueprintFunctionLibrary::GetPlayerStatsMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetPlayerStatsMenuWidgetController(WCParams);
	}
	
	return nullptr;
}

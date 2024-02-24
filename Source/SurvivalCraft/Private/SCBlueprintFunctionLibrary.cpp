// Copyright Fillipe Romero

#include "SCBlueprintFunctionLibrary.h"
#include "Interfaces/CombatInterface.h"
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

USCTribeWidgetController* USCBlueprintFunctionLibrary::GetTribeWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetTribeWidgetController(WCParams);
	}
	
	return nullptr;
}

USCChatWidgetController* USCBlueprintFunctionLibrary::GetChatWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ASCHUD* SCHUD = nullptr;
	
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, SCHUD))
	{
		return SCHUD->GetChatWidgetController(WCParams);
	}
	
	return nullptr;
}

float USCBlueprintFunctionLibrary::CalculateDamage(AActor* Target, const float BaseDamage)
{
	float Damage = BaseDamage * 0.03;
	
	if (Target->Implements<UCombatInterface>())
	{
		const int32 ArmorAmount = ICombatInterface::Execute_GetArmorAmount(Target);
		float DamageMultiplier = 4.f;
		
		if (ArmorAmount == 1) DamageMultiplier = 3.5f;
		else if (ArmorAmount == 2) DamageMultiplier = 3.0f;
		else if (ArmorAmount == 3) DamageMultiplier = 2.5f;
		else if (ArmorAmount == 4) DamageMultiplier = 2.0f;

		Damage *= DamageMultiplier;
	}
	else
	{
		Damage *= 4.f;
	}

	return Damage;
}

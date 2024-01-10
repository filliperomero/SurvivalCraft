// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SCBlueprintFunctionLibrary.generated.h"

class USCPlayerStatsMenuWidgetController;
class USCCraftingMenuWidgetController;
class USCHotbarMenuWidgetController;
class USCInventoryMenuWidgetController;
class USCOverlayWidgetController;
class ASCHUD;

UCLASS()
class SURVIVALCRAFT_API USCBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "SCBlueprintFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ASCHUD*& OutSCHUD);
	
	UFUNCTION(BlueprintPure, Category = "SCBlueprintFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USCOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "SCBlueprintFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USCInventoryMenuWidgetController* GetInventoryMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SCBlueprintFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USCHotbarMenuWidgetController* GetHotbarMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SCBlueprintFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USCCraftingMenuWidgetController* GetCraftingMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "SCBlueprintFunctionLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USCPlayerStatsMenuWidgetController* GetPlayerStatsMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "SCBlueprintFunctionLibrary|Damage")
	static float CalculateDamage(AActor* Target, const float BaseDamage);
};

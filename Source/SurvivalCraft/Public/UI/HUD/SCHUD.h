// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCHUD.generated.h"

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
};

// Copyright Fillipe Romero


#include "UI/HUD/SCHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/SCUserWidget.h"
#include "UI/WidgetController/SCOverlayWidgetController.h"
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

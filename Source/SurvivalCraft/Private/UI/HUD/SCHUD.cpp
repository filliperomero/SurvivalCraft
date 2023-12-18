// Copyright Fillipe Romero


#include "UI/HUD/SCHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget/SCUserWidget.h"

void ASCHUD::InitOverlay(APlayerController* PC)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_SCHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<USCUserWidget>(Widget);

	Widget->AddToViewport();
}

// Copyright Fillipe Romero

#include "UI/Widget/SCUserWidget.h"

void USCUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

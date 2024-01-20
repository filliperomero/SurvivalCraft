// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCBuildableInteractTextWidget.generated.h"

UCLASS()
class SURVIVALCRAFT_API USCBuildableInteractTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWidgetInfo(ESlateVisibility WidgetVisibility, bool bShowInteractText, bool bShowOptionsText, const FText& StructureName, const FText& OwnerName, float CurrentHealth, float MaxHealth);
};

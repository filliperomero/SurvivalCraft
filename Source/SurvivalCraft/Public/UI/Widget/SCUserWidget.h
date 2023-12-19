// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SCUserWidget.generated.h"

UCLASS()
class SURVIVALCRAFT_API USCUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};

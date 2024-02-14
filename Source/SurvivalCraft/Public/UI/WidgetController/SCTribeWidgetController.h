// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCTribeWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCTribeWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void CreateTribe(const FText TribeName);
};

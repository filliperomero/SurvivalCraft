// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCPlayerStatsMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsChangedSignature, float, NewValue);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCPlayerStatsMenuWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnFoodChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnMaxFoodChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnWaterChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnMaxWaterChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnStaminaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats")
	FOnStatsChangedSignature OnMaxStaminaChangedDelegate;
};

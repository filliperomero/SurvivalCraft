// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCPlayerStatsMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatsChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnXPChangedSignature, int32, CurrentXP, int32, RequiredXP, float, XPPercent);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCPlayerStatsMenuWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnMaxHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnFoodChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnMaxFoodChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnWaterChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnMaxWaterChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnStaminaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Attributes")
	FOnStatsChangedSignature OnMaxStaminaChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|XP")
	FOnXPChangedSignature OnXPChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Player Stats|Skill Points")
	FOnStatsChangedSignature OnSkillPointsChangedDelegate;
};

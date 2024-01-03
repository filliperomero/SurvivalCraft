// Copyright Fillipe Romero

#include "UI/WidgetController/SCPlayerStatsMenuWidgetController.h"
#include "Player/SCPlayerController.h"

void USCPlayerStatsMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	OnHealthChangedDelegate.Broadcast(GetSCPC()->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(GetSCPC()->GetMaxHealth());
	OnFoodChangedDelegate.Broadcast(GetSCPC()->GetFood());
	OnMaxFoodChangedDelegate.Broadcast(GetSCPC()->GetMaxFood());
	OnWaterChangedDelegate.Broadcast(GetSCPC()->GetWater());
	OnMaxWaterChangedDelegate.Broadcast(GetSCPC()->GetMaxWater());
	OnStaminaChangedDelegate.Broadcast(GetSCPC()->GetStamina());
	OnMaxStaminaChangedDelegate.Broadcast(GetSCPC()->GetMaxStamina());
}

void USCPlayerStatsMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnPlayerHealthChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnHealthChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerWaterChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnWaterChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerFoodChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnFoodChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerStaminaChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnStaminaChangedDelegate.Broadcast(NewValue);
		}
	);
}

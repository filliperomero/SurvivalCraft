// Copyright Fillipe Romero

#include "UI/WidgetController/SCPlayerStatsMenuWidgetController.h"
#include "Player/SCPlayerController.h"

void USCPlayerStatsMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	
	OnHealthChangedDelegate.Broadcast(GetSCPC()->GetHealth());
	OnMaxHealthChangedDelegate.Broadcast(GetSCPC()->GetMaxHealth());
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
}

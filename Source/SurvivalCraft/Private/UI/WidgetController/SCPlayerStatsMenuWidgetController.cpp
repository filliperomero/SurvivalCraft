﻿// Copyright Fillipe Romero

#include "UI/WidgetController/SCPlayerStatsMenuWidgetController.h"
#include "Data/LevelUpInfo.h"
#include "Player/SCPlayerController.h"
#include "Player/SCPlayerState.h"

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

	GetSCPS()->OnXPChangedDelegate.AddLambda(
		[this](int32 NewXP, int32 EarnedXP)
		{
			const ULevelUpInfo* LevelUpInfo = GetSCPS()->LevelUpInfo;

			checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo. Please fill out SCPlayerState Blueprint"));

			const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
			const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

			if (Level <= MaxLevel && Level > 0)
			{
				const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
				const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

				const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
				const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

				const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
				
				OnXPChangedDelegate.Broadcast(XPForThisLevel, DeltaLevelUpRequirement, XPBarPercent);
			}
		}
	);
	GetSCPS()->OnSkillPointsChangedDelegate.AddLambda(
		[this](int32 NewSkillPoints)
		{
			OnSkillPointsChangedDelegate.Broadcast(static_cast<float>(NewSkillPoints));
		}
	);

	GetSCPC()->OnPlayerHealthChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnHealthChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerMaxHealthChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnMaxHealthChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerWaterChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnWaterChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerMaxWaterChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnMaxWaterChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerFoodChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnFoodChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerMaxFoodChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnMaxFoodChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerStaminaChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnStaminaChangedDelegate.Broadcast(NewValue);
		}
	);
	GetSCPC()->OnPlayerMaxStaminaChangedDelegate.AddLambda(
		[this](float NewValue)
		{
			OnMaxStaminaChangedDelegate.Broadcast(NewValue);
		}
	);
}

void USCPlayerStatsMenuWidgetController::SpendPointButtonPressed(EPlayerStats StatToUpgrade)
{
	if (GetSCPC() == nullptr) return;

	// Avoid calling a Server RPC if locally we don't have enough points. We'll still validate in the server
	if (GetSCPS() == nullptr || GetSCPS()->GetSkillPoints() <= 0) return;

	GetSCPC()->ServerSpendSkillPoint(StatToUpgrade);
}

// Copyright Fillipe Romero

#include "Player/SCPlayerState.h"
#include "Data/LevelUpInfo.h"
#include "Net/UnrealNetwork.h"

ASCPlayerState::ASCPlayerState()
{
	NetUpdateFrequency = 100.f;
}

void ASCPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCPlayerState, Level);
	DOREPLIFETIME(ASCPlayerState, XP);
	DOREPLIFETIME(ASCPlayerState, SkillPoints);
	DOREPLIFETIME(ASCPlayerState, bIsInTribe);
	DOREPLIFETIME(ASCPlayerState, PlayerName);
	DOREPLIFETIME(ASCPlayerState, TribeID);
	DOREPLIFETIME(ASCPlayerState, TribeName);
	DOREPLIFETIME(ASCPlayerState, TribeRank);
}

void ASCPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ASCPlayerState::AddToXP(int32 InXP)
{
	if (InXP == 0) return;
	
	const int32 CurrentLevel = Level;
	const int32 CurrentXP = XP;
	const int32 NewLevel = LevelUpInfo->FindLevelForXP(CurrentXP + InXP);
	const int32 NumLevelUps = NewLevel - CurrentLevel;
	
	if (NumLevelUps > 0)
	{
		AddToLevel(NumLevelUps);
		int32 SkillPointsReward = 0;

		for (int32 Index = 0; Index < NumLevelUps; Index++)
		{
			SkillPointsReward += LevelUpInfo->LevelUpInformation[CurrentLevel + Index].SkillPointAward;
		}
		
		if (SkillPointsReward > 0) AddToSkillPoints(SkillPointsReward);
	}
	
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP, InXP);
}

void ASCPlayerState::AddToSkillPoints(int32 InPoints)
{
	SkillPoints += InPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void ASCPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ASCPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP, XP - OldXP);
}

void ASCPlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

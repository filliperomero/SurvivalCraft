// Copyright Fillipe Romero

#include "Game/SCGameState.h"
#include "Player/SCPlayerController.h"
#include "Player/SCPlayerState.h"

void ASCGameState::CreateTribe(const FTribeInfo& Tribe)
{
	// Already exist a tribe with this ID
	if (TribeMap.Find(Tribe.ID) != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("A tribe with this ID (%s) already exist."), *Tribe.ID)
		return;
	}
	
	TribeMap.Add(Tribe.ID, Tribe);
}

FTribeInfo* ASCGameState::GetTribeByID(const FString& TribeID)
{
	return TribeMap.Find(TribeID);
}

bool ASCGameState::UpdateTribeByID(const FString& TribeID, const FTribeInfo& TribeInfo)
{
	FTribeInfo* Tribe = GetTribeByID(TribeID);

	if (Tribe == nullptr) return false;

	Tribe->Logs = TribeInfo.Logs;
	Tribe->Members = TribeInfo.Members;
	Tribe->Name = TribeInfo.Name;
	Tribe->DayMessage = TribeInfo.DayMessage;

	UpdateTribeInfoOnClients(TribeInfo);

	return true;
}

void ASCGameState::AddLogToTribe(const FString& TribeID, const FTribeLogEntry& LogEntry)
{
	const FTribeInfo* Tribe = GetTribeByID(TribeID);

	if (Tribe == nullptr) return;
	
	FTribeInfo TribeToUpdate = *Tribe;

	TribeToUpdate.Logs.Add(LogEntry);

	UpdateTribeByID(TribeID, TribeToUpdate);
}

bool ASCGameState::DemoteTribeMember(const FString& TribeID, const FString& MemberIDToDemote, const FText& PlayerWhoInitiated)
{
	if (!HasAuthority()) return false;
	
	const FTribeInfo* Tribe = GetTribeByID(TribeID);

	if (Tribe == nullptr) return false;

	FTribeInfo LocalTribe = *Tribe;
	int32 IndexToUpdate = -1;

	for (int32 Index = 0; Index < LocalTribe.Members.Num(); Index++)
	{
		if (LocalTribe.Members[Index].PlayerIDOffline.Equals(MemberIDToDemote) && LocalTribe.Members[Index].TribeRank == ETribeRank::ETR_Admin)
		{
			IndexToUpdate = Index;
			break;
		}
	}

	if (IndexToUpdate == -1) return false;

	LocalTribe.Members[IndexToUpdate].TribeRank = ETribeRank::ETR_Member;

	FTribeLogEntry LogEntry;
	LogEntry.Day = FText::FromString(FString::Printf(TEXT("1")));
	LogEntry.Time = FText::FromString(FString::SanitizeFloat(GetGameTimeSinceCreation()));
	LogEntry.LogColor = ETribeLogColor::ETL_Yellow;
	LogEntry.LogText = FText::FromString(FString::Printf(TEXT("%s Demoted %s to Member Rank"), *PlayerWhoInitiated.ToString(), *LocalTribe.Members[IndexToUpdate].PlayerName.ToString()));

	LocalTribe.Logs.Add(LogEntry);
	
	if (UpdateTribeByID(TribeID, LocalTribe))
	{
		if (IsValid(LocalTribe.Members[IndexToUpdate].PlayerController))
		{
			if (ASCPlayerState* DemotedPlayerState = LocalTribe.Members[IndexToUpdate].PlayerController->GetPlayerState<ASCPlayerState>())
			{
				DemotedPlayerState->SetTribeRank(ETribeRank::ETR_Member);
			}
		}
		
		return true;
	}

	return false;
}

void ASCGameState::UpdateTribeInfoOnClients(const FTribeInfo& TribeInfo)
{
	for (const FTribeMemberInfo& Member : TribeInfo.Members)
	{
		if (IsValid(Member.PlayerController) && Member.bIsOnline)
		{
			Member.PlayerController->ClientUpdateTribeInfo(TribeInfo, false);
		}
	}
}

// Copyright Fillipe Romero

#include "Game/SCGameState.h"
#include "Player/SCPlayerController.h"

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

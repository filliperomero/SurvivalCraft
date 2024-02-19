// Copyright Fillipe Romero

#include "UI/WidgetController/SCTribeWidgetController.h"
#include "Player/SCPlayerController.h"

void USCTribeWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnUpdateTribeDelegate.AddLambda(
		[this](const FTribeInfo& TribeInfo)
		{
			OnUpdateTribeWidgetDelegate.Broadcast(TribeInfo);
		}
	);

	GetSCPC()->OnReceiveTribeInviteDelegate.AddLambda(
		[this](const FString& TribeID, const FText& TribeName, const FText& SenderName)
		{
			InviteTribeID = TribeID;
			InviteSenderName = SenderName;
		}
	);
}

void USCTribeWidgetController::CreateTribe(const FText TribeName)
{
	GetSCPC()->ServerCreateTribe(TribeName);
}

void USCTribeWidgetController::AcceptTribeInvite()
{
	if (InviteTribeID.IsEmpty() || InviteSenderName.IsEmpty()) return;
	
	GetSCPC()->ServerJoinTribe(InviteTribeID, InviteSenderName);

	// Reset variables
	InviteTribeID = FString();
	InviteSenderName = FText();
}

void USCTribeWidgetController::DemoteTribeMember()
{
	GetSCPC()->ServerDemoteTribeMember(SelectedMemberID);
}

void USCTribeWidgetController::PromoteTribeMember()
{
	GetSCPC()->ServerPromoteTribeMember(SelectedMemberID);
}

void USCTribeWidgetController::KickTribeMember()
{
	GetSCPC()->ServerKickTribeMember(SelectedMemberID);
}

void USCTribeWidgetController::LeaveTribe()
{
	GetSCPC()->ServerLeaveTribe();
}

void USCTribeWidgetController::SetTribeMessage(const FText& Message)
{
	GetSCPC()->ServerSetTribeMessage(Message);
}

void USCTribeWidgetController::MemberSlotSelected(const FString& MemberID)
{
	SelectedMemberID = MemberID;
}

void USCTribeWidgetController::MemberSlotDeselected()
{
	SelectedMemberID = FString();
}

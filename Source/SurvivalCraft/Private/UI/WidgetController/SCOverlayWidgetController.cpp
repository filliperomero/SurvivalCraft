// Copyright Fillipe Romero

#include "UI/WidgetController/SCOverlayWidgetController.h"
#include "Player/SCPlayerController.h"
#include "Player/SCPlayerState.h"

void USCOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	GetSCPC()->OnToggleInventoryDelegate.AddLambda(
		[this]()
		{
			OnToggleInventoryWidgetDelegate.Broadcast();
		}
	);

	GetSCPC()->OnItemAddedDelegate.AddLambda(
		[this](UTexture2D* ItemIcon, int32 ItemQuantity, FText ItemName)
		{
			OnItemAddedWidgetDelegate.Broadcast(ItemIcon, ItemQuantity, ItemName);
		}
	);

	GetSCPC()->OnDemolishStructureDelegate.AddLambda(
		[this](bool bCancelDemolish, float DemolishTime)
		{
			OnDemolishStructureWidgetDelegate.Broadcast(bCancelDemolish, DemolishTime);
		}
	);

	GetSCPC()->OnShowItemOptionsMenuDelegate.AddLambda(
		[this](int32 Index, EContainerType Container)
		{
			OnShowItemOptionsWidgetDelegate.Broadcast(Index, Container);
		}
	);

	GetSCPC()->OnHideItemOptionsMenuDelegate.AddLambda(
		[this]()
		{
			OnHideItemOptionsWidgetDelegate.Broadcast();
		}
	);

	GetSCPC()->OnToggleMenuOptionsWidgetDelegate.AddLambda(
		[this](EMenuOptionsWidgetType WidgetToShow, bool bIsInTribe)
		{
			OnToggleMenuOptionsDelegate.Broadcast(WidgetToShow, bIsInTribe);
		}
	);

	GetSCPC()->OnReceiveTribeInviteDelegate.AddLambda(
		[this](const FString& TribeID, const FText& TribeName, const FText& SenderName)
		{
			OnReceiveTribeInviteWidgetDelegate.Broadcast(TribeName, SenderName);
		}
	);

	GetSCPS()->OnXPChangedDelegate.AddLambda(
		[this](int32 NewXP, int32 EarnedXP)
		{
			OnXPChangedDelegate.Broadcast(static_cast<float>(EarnedXP));
		}
	);

	GetSCPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnLevelChangedDelegate.Broadcast(static_cast<float>(NewLevel));
		}
	);
}

void USCOverlayWidgetController::HideItemOptionsMenu()
{
	OnHideItemOptionsWidgetDelegate.Broadcast();
}

void USCOverlayWidgetController::DropItem(EContainerType ContainerType, int32 FromIndex)
{
	GetSCPC()->DropItem(ContainerType, FromIndex);
}

void USCOverlayWidgetController::SplitItemStack(EContainerType ContainerType, int32 FromIndex)
{
	GetSCPC()->SplitItemStack(ContainerType, FromIndex);
}

void USCOverlayWidgetController::ToggleMenuOptions(EMenuOptionsWidgetType WidgetToShow)
{
	GetSCPC()->ClientToggleMenuOptionsWidget(WidgetToShow);
}

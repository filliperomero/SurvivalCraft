// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "Tribe/SCTribeData.h"
#include "SCTribeWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateTribeWidgetSignature, const FTribeInfo&, TribeInfo);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCTribeWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateTribeWidgetSignature OnUpdateTribeWidgetDelegate;

	UFUNCTION(BlueprintCallable)
	void CreateTribe(const FText TribeName);

	UFUNCTION(BlueprintCallable)
	void AcceptTribeInvite();

	UFUNCTION(BlueprintCallable)
	void DemoteTribeMember();

	UFUNCTION(BlueprintCallable)
	void PromoteTribeMember();

	UFUNCTION(BlueprintCallable)
	void KickTribeMember();

	UFUNCTION(BlueprintCallable)
	void LeaveTribe();

	UFUNCTION(BlueprintCallable)
	void SetTribeMessage(const FText& Message);
	
	UFUNCTION(BlueprintCallable)
	void MemberSlotSelected(const FString& MemberID);

	UFUNCTION(BlueprintCallable)
	void MemberSlotDeselected();

private:
	FString InviteTribeID = FString();
	FText InviteSenderName = FText();
	FString SelectedMemberID = FString();
};

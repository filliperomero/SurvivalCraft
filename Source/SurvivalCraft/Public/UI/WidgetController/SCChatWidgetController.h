// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCWidgetController.h"
#include "SCChatWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterPressedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnReceiveChatMessageWidgetSignature, const FString&, Message, const FText&, TribeName, const FText&, PlayerName, bool, bIsGlobalMessage);

UCLASS(BlueprintType, Blueprintable)
class SURVIVALCRAFT_API USCChatWidgetController : public USCWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnEnterPressedSignature OnEnterPressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnReceiveChatMessageWidgetSignature OnReceiveChatMessageWidgetDelegate;

	UFUNCTION(BlueprintCallable)
	void SendChatMessage(const FString& Message, bool bIsGlobalMessage);
};

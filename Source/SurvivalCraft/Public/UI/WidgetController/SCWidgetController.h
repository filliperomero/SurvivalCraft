// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SCWidgetController.generated.h"

class ASCPlayerState;
class ASCPlayerController;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS)
	: PlayerController(PC), PlayerState(PS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
};

UCLASS()
class SURVIVALCRAFT_API USCWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	
	virtual void BindCallbacksToDependencies();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<ASCPlayerController> SCPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Widget Controller")
	TObjectPtr<ASCPlayerState> SCPlayerState;
	
	ASCPlayerController* GetSCPC();
	ASCPlayerState* GetSCPS();
};

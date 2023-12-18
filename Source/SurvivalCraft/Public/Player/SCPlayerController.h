// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class SURVIVALCRAFT_API ASCPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputMappingContext;
};

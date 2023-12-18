// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SCHUD.generated.h"

class USCUserWidget;

UCLASS()
class SURVIVALCRAFT_API ASCHUD : public AHUD
{
	GENERATED_BODY()

public:
	void InitOverlay(APlayerController* PC);

private:
	UPROPERTY()
	TObjectPtr<USCUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USCUserWidget> OverlayWidgetClass;
};

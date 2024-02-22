// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCBuildable.h"
#include "SCRoof.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCRoof : public ASCBuildable
{
	GENERATED_BODY()

public:
	ASCRoof();
	virtual void DestroyStructure(const bool bLog) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> RoofBox;
};

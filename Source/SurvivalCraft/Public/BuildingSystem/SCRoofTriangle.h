// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "SCBuildable.h"
#include "SCRoofTriangle.generated.h"

UCLASS()
class SURVIVALCRAFT_API ASCRoofTriangle : public ASCBuildable
{
	GENERATED_BODY()

public:
	ASCRoofTriangle();
	virtual void DestroyStructure(const bool bLog) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> RoofBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> RoofBox2;
};

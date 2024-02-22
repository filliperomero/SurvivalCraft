﻿// Copyright Fillipe Romero

#pragma once

#include "CoreMinimal.h"
#include "Data/BuildableData.h"
#include "GameFramework/Actor.h"
#include "SCBuildable.generated.h"

class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class SURVIVALCRAFT_API ASCBuildable : public AActor
{
	GENERATED_BODY()

public:
	ASCBuildable();
	virtual void DestroyStructure(const bool bLog);
	virtual void HandleDestroyStructure();
	virtual void DemolishStructure(const FText& PlayerName);

	UFUNCTION(Client, Reliable)
	void ClientShowInteractText(ESlateVisibility WidgetVisibility, bool bShowInteractText, bool bShowOptionsText, const FText& InStructureName, const FText& InOwnerName, float CurrentHealth, float InMaxHealth);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTextSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTextSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastDestroyStructure();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	TObjectPtr<UGeometryCollectionComponent> DestructibleMesh;

	UPROPERTY(VisibleAnywhere, Category = "Collision Box")
	TObjectPtr<UBoxComponent> OverlapBox;

	UPROPERTY(VisibleAnywhere, Category = "Text Overlap Box")
	TObjectPtr<USphereComponent> TextOverlapSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget)
	TObjectPtr<UWidgetComponent> InfoWidget;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	FBuildableInfo BuildableInfo;

	UPROPERTY(VisibleAnywhere, Category = "Collision Box" )
	TArray<UBoxComponent*> SnapBoxes;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buildable Properties", meta = (AllowPrivateAccess = true))
	EStructureDamageType StructureDamageType = EStructureDamageType::None;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	bool bIsInteractable = false;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	bool bHasOptions = true;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	bool bCanDemolish = true;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	FText StructureName = FText();
	
	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	FText OwnerName = FText();

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	float LifeSpan = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "Buildable Properties")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Buildable Properties")
	float MaxHealth = 100.f;

	FString TribeID = FString();
	FString OwnerNetID = FString();

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE FBuildableInfo GetBuildableInfo() const { return BuildableInfo; }
	FORCEINLINE UBoxComponent* GetOverlapBox() const { return OverlapBox; }
	FORCEINLINE TArray<UBoxComponent*> GetSnapBoxes() const { return SnapBoxes; }
	FORCEINLINE FString GetTribeID() const { return TribeID; }
	FORCEINLINE FString GetOwnerNetID() const { return OwnerNetID; }
	FORCEINLINE void SetTribeID(const FString& InTribeID) { TribeID = InTribeID; }
	FORCEINLINE void SetOwnerNetID(const FString& InOwnerNetID) { OwnerNetID = InOwnerNetID; }
	FORCEINLINE void SetOwnerName(const FText& InOwnerName) { OwnerName = InOwnerName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetSnapBoxes(const TArray<UBoxComponent*>& InSnapBoxes) { SnapBoxes = InSnapBoxes; }
};

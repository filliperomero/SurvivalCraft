// Copyright Fillipe Romero

#include "Items/SCArmor.h"

ASCArmor::ASCArmor()
{
	PrimaryActorTick.bCanEverTick = false;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetOwnerNoSee(true);
}

void ASCArmor::SetMasterPose(USkeletalMeshComponent* CharacterMesh) const
{
	SkeletalMesh->SetLeaderPoseComponent(CharacterMesh);
}

// TODO: Check the necessity of setting the ArmorItemInfo
void ASCArmor::OnConstruction(const FTransform& Transform)
{
	if (ItemsDataTable == nullptr || ItemRowName.IsNone()) return;

	if (const FItemInformation* ItemInformation = ItemsDataTable->FindRow<FItemInformation>(ItemRowName, TEXT("")))
	{
		ArmorItemInfo = *ItemInformation;
	}
}

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

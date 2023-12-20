// Copyright Fillipe Romero

#include "SurvivalCraft/Public/Character/SCCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/SCPlayerInventoryComponent.h"
#include "Player/SCPlayerController.h"
#include "UI/HUD/SCHUD.h"

DEFINE_LOG_CATEGORY(LogSCCharacter);

ASCCharacter::ASCCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	GetMesh()->SetOwnerNoSee(true);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(0.f, -90.0f, 0.f));
	Mesh1P->SetRelativeLocation(FVector(00.f, 0.f, -160.f));

	InventoryComponent = CreateDefaultSubobject<USCPlayerInventoryComponent>(TEXT("InventoryComponent"));
}

ASCPlayerController* ASCCharacter::GetSCPlayerController_Implementation()
{
	// TODO: create a variable for it and return it
	return GetController<ASCPlayerController>();
}

void ASCCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize Inventory
	const FItemInformation Item = FItemInformation();
	InventoryComponent->InitializeItems(Item, 30);
}

void ASCCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// In multiplayer, the PlayerController will be invalid in other people computers, since I can only get my PlayerController but not others
	if (ASCPlayerController* SCPlayerController = Cast<ASCPlayerController>(GetController()))
	{
		// The HUD is only valid to locally controller player
		if (ASCHUD* HUD = Cast<ASCHUD>(SCPlayerController->GetHUD()))
		{
			HUD->InitOverlay(SCPlayerController, GetPlayerState());
		}
	}
}

// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCArrowProjectile.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystemInstanceController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCArrowProjectile::ASCArrowProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetBoxExtent(FVector(30.f, 10.f, 10.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Block);
	CollisionBox->bReturnMaterialOnMove = true;
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = 4000.f;
	ProjectileMovementComponent->MaxSpeed = 4000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.3f;
}

void ASCArrowProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	
	SpawnTrailSystem();
	SetLifeSpan(3.f);
}

void ASCArrowProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetInstigator()) return;

	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);

	AttachToActor(OtherActor, AttachmentRules);

	if (HasAuthority())
	{
		if (const APawn* Pawn = Cast<APawn>(GetInstigator()))
		{
			float FinalDamage = Damage * DamageMultiplier;
			
			if (OtherActor->Implements<UPlayerInterface>())
			{
				switch (Hit.PhysMaterial->SurfaceType)
				{
				case SURFACE_HEAD:
					FinalDamage *= 8.f;
					break;
				case SURFACE_CHEST:
					FinalDamage *= 5.f;
					break;
				case SURFACE_LEGS:
					FinalDamage *= 3.f;
					break;
					// No change on the Damage for Boots, Limb and Default
				case SURFACE_BOOTS:
				case SURFACE_LIMB:
				default: 
					break;
				}

				if (IPlayerInterface::Execute_HasArmorInSlot(OtherActor, Hit.PhysMaterial->SurfaceType))
				{
					FinalDamage -= FinalDamage * 0.25f;
				}
			}
			else
			{
				FinalDamage *= 4.f;
				// It is something else
			}
			
			UGameplayStatics::ApplyDamage(OtherActor, FinalDamage, Pawn->GetController(), GetOwner(), UDamageType::StaticClass());
		}
	}

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (TrailSystemComponent && TrailSystemComponent->GetSystemInstanceController())
	{
		TrailSystemComponent->GetSystemInstanceController()->Deactivate();
	}
}

void ASCArrowProjectile::SpawnTrailSystem()
{
	if (TrailSystem) {
		TrailSystemComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailSystem,
			GetRootComponent(),
			FName(),
			GetActorLocation(),
			GetActorRotation(),
			EAttachLocation::KeepWorldPosition,
			false
		);
	}
}

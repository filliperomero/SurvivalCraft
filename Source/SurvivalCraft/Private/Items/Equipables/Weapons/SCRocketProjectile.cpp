// Copyright Fillipe Romero

#include "Items/Equipables/Weapons/SCRocketProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "SurvivalCraft/SurvivalCraft.h"

ASCRocketProjectile::ASCRocketProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetBoxExtent(FVector(15.f, 5.f, 8.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_STRUCTURE, ECR_Block);
	
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ASCRocketProjectile::BeginPlay()
{
	Super::BeginPlay();

	SpawnTrailSystem();
	
	CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	
	SetLifeSpan(5.f);
}

void ASCRocketProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetInstigator()) return;

	StartDestroyTimer();
	
	if (HasAuthority())
	{
		if (const APawn* Pawn = Cast<APawn>(GetInstigator()))
		{
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				this,
				Damage,
				10.f,
				GetActorLocation(),
				DamageInnerRadius,
				DamageOuterRadius,
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				GetOwner(),
				Pawn->GetController()
			);
		}
	}

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	ProjectileMesh->SetVisibility(false);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (TrailSystemComponent)
	{
		TrailSystemComponent->Deactivate();
	}
}

void ASCRocketProjectile::SpawnTrailSystem()
{
	if (TrailSystem == nullptr) return;

	TrailSystemComponent = UGameplayStatics::SpawnEmitterAttached(
		TrailSystem,
		ProjectileMesh,
		FName(),
		GetActorLocation(),
		GetActorRotation(),
		EAttachLocation::KeepWorldPosition,
		false
	);
}

void ASCRocketProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::DestroyTimerFinished, DestroyDelay);
}

void ASCRocketProjectile::DestroyTimerFinished()
{
	Destroy();
}

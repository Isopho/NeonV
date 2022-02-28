// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Damageable.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));
	// Create projectile movement component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 600;
	ProjectileMovement->MaxSpeed = 600; 
	ProjectileMovement->ProjectileGravityScale = 0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileRoot"));

	ProjectileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetupAttachment(GetRootComponent());
	ProjectileCollision->SetCollisionProfileName(FName("OverlapAllDynamic"), true);	 //TODO Add OverlapProfile for projectiles
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileBeginOverlap);
	ProjectileCollision->OnComponentEndOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileEndOverlap);

	Tags.AddUnique(ProjectileTag);
	IgnoreCollisionTags.AddUnique(ProjectileTag);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseProjectile::OnProjectileBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep, const
	FHitResult& SweepResult
)
{
	// Check if the projectile should pass through the Actor
	for (FName IgnoreCollisionTag : IgnoreCollisionTags) 
	{

		if (OtherActor->ActorHasTag(IgnoreCollisionTag)) 
		{
			return;
		}
	}

	// Damage hit Actor if possible
	IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
	if (DamageableActor)
	{
		IDamageable::Execute_AffectHealth(OtherActor, Damage * -1.0f);
	}

	OnProjectileCollision(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void ABaseProjectile::OnProjectileEndOverlap
(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	// nothing to do
}

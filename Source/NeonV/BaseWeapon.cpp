// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponRoot"));

	// Create a projectile spawn
	ProjectileSpawn = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawn")); 
	ProjectileSpawn->SetupAttachment(GetRootComponent());

	Tags.AddUnique(WeaponTag);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFireTrigger && !GetWorldTimerManager().IsTimerActive(WeaponCooldownHandle))
	{
		bWeaponOnCooldown = true;
		FireWeapon();		

		GetWorldTimerManager().SetTimer(
			WeaponCooldownHandle, this, &ABaseWeapon::WeaponOffCooldown, GetWeaponCooldown(), false);
	}
}

void ABaseWeapon::FireTrigger()
{
	bFireTrigger = true;
}

void ABaseWeapon::ReleaseFireTrigger()
{
	bFireTrigger = false;
}

void ABaseWeapon::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("FireWeapon"));

	FTransform SpawnTransform = ProjectileSpawn->GetComponentTransform();
	SpawnTransform.SetScale3D(ProjectileSize);

	ABaseProjectile* Projectile = Cast<ABaseProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (Projectile->GetClass() != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("FireWeapon - Projectile Class: " + Projectile->GetClass()->GetName()));
		//Projectile Init
		Projectile->IgnoreCollisionTags.Append(ProjectileIgnoreCollisionTags);
		Projectile->SetInstigator(this->GetInstigator());

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	}

	WeaponFired();
}

void ABaseWeapon::WeaponFired_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("WeaponFired_Implementation"));
}

void ABaseWeapon::WeaponOffCooldown()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("WeaponOffCooldown"));
	bWeaponOnCooldown = false;
}

float ABaseWeapon::GetWeaponCooldown()
{
	return 60 / RoundsPerMinute;
}

float ABaseWeapon::GetElapsedWeaponCooldown()
{
	return UKismetMathLibrary::FClamp(GetWorldTimerManager().GetTimerElapsed(WeaponCooldownHandle), 0.0f, 3600.0f);
}

float ABaseWeapon::GetRemainingWeaponCooldown()
{
	return UKismetMathLibrary::FClamp(GetWorldTimerManager().GetTimerRemaining(WeaponCooldownHandle), 0.0f, 3600.0f);
}


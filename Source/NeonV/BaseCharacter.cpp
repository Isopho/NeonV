// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Util/NeonVMathUtil.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Determin CharacterCenter component.
	TArray<UActorComponent*> CharacterCenterComponents = GetComponentsByTag(USceneComponent::StaticClass(), CharacterCenterTag);

	if (CharacterCenterComponents.IsEmpty())
	{
		CharacterCenterComponent = GetRootComponent(); UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("No component has been tagged as CharacterCenter on " + GetName() + "! Using root component as CharacterCenter."));
	}
	else
	{
		CharacterCenterComponent = (USceneComponent*)CharacterCenterComponents[0];

		if (CharacterCenterComponents.Num() > 1) {
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FString("Multiple components have been tagged as CharacterCenter on " + GetName() + "! Using " + CharacterCenterComponent->GetName() + " as CharacterCenter."));
		}
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AdjustCharacterOrientation();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//imnpl calc health
void ABaseCharacter::CalculateHealth(float DeltaHealth)
{
	Health += DeltaHealth;
	CalculateDead();
}

//imnpl calc health
void ABaseCharacter::AffectHealth_Implementation(float DeltaHealth)
{
	CalculateHealth(DeltaHealth);
	HealthAffected(DeltaHealth);
}


void ABaseCharacter::CreateAndEquipWeapon(USceneComponent * WeaponMount, TSubclassOf<ABaseWeapon> WeaponClass)
{
	FTransform SpawnTransform = WeaponMount->GetComponentTransform();

	ABaseWeapon * Weapon = Cast<ABaseWeapon>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, WeaponClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, this));
	if (Weapon)
	{
		//Weapon Init
		Weapon->SetInstigator(this);

		UGameplayStatics::FinishSpawningActor(Weapon, SpawnTransform);
	}

	Weapons.Add(Weapon);

	Weapon->AttachToComponent(WeaponMount, FAttachmentTransformRules::SnapToTargetIncludingScale);
}


void ABaseCharacter::FireTrigger()
{
	for (ABaseWeapon* Weapon : Weapons)
	{
		Weapon->FireTrigger();
	}
}

void ABaseCharacter::ReleaseFireTrigger()
{
	for (ABaseWeapon* Weapon : Weapons)
	{
		Weapon->ReleaseFireTrigger();
	}
}

// calc dead
void ABaseCharacter::CalculateDead() 
{
	if (!bDead && Health <= 0)
	{
		bDead = true;
		OnDeath();
	}
}

void ABaseCharacter::AdjustCharacterOrientation()
{
	FVector DesiredOrientation = CalculateDesiredOrientation();
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString("ABaseCharacter::AdjustCharacterOrientation " + DesiredOrientation.ToString()));
	float AngleToDesiredOrientation = NeonVMath::CalculateHorizontalAngle(CharacterCenterComponent->GetForwardVector(), DesiredOrientation);
	TurnCharacter(AngleToDesiredOrientation);
}

FVector ABaseCharacter::CalculateDesiredOrientation()
{
	// Dummy Implementation -> No turning
	return CharacterCenterComponent->GetForwardVector();
}

void ABaseCharacter::TurnCharacter(float AngleToDesiredOrientation)
{
	// Reduce turnrate for "small" angles. (Angles within the SoftTurnRadian)
	float SoftTurnRateMultiplier = UKismetMathLibrary::FClamp(AngleToDesiredOrientation / SoftTurnRadian, -1.0f, 1.0f);

	AddControllerYawInput(SoftTurnRateMultiplier * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

#if WITH_EDITOR
void ABaseCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) 
{
	bDead = false;
	Health = 100;

	Super::PostEditChangeProperty(PropertyChangedEvent);

	CalculateDead();
}
#endif
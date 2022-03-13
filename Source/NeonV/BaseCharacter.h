// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseWeapon.h"
#include "Damageable.h"
#include "BaseCharacter.generated.h"
	
UCLASS(Blueprintable)
class NEONV_API ABaseCharacter : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	// ======================================================================================
	//	public Properties
	// ======================================================================================

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Character")
		float Health = 100;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Base Character")
		bool bDead = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Base Character")
		TArray<ABaseWeapon *> Weapons;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base Character")
		FName CharacterCenterTag = FName("CharacterCenter");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Character")
		float BaseTurnRate = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Character")
		float SoftTurnRadian = 30;

	// ======================================================================================
	//	Health & Death Functions
	// ======================================================================================
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		virtual void CalculateHealth(float DeltaHealth);

	virtual void AffectHealth_Implementation(float DeltaHealth) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Character")
		void HealthAffected(float DeltaHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Character")
		void OnDeath();

	// ======================================================================================
	//	Weapon Functions
	// ======================================================================================

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		virtual void CreateAndEquipWeapon(USceneComponent * WeaponMount, TSubclassOf<ABaseWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		virtual void FireTrigger();

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		virtual void ReleaseFireTrigger();

	// ======================================================================================
	//	General Functions
	// ======================================================================================

#if WITH_EDITOR
	//editor-centric code for changing properties
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// Sets default values for this character's properties
	ABaseCharacter();

protected:

	// ======================================================================================
	//	protected Properties
	// ======================================================================================

	USceneComponent* CharacterCenterComponent;


	virtual void BeginPlay() override;

	//Calc death function (helper)
	virtual void CalculateDead();

	virtual void AdjustCharacterOrientation();

	virtual FVector CalculateDesiredOrientation();

	virtual void TurnCharacter(float AngleToDesiredOrientation);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};



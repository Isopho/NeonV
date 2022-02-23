// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class NEONV_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Base Weapon")
		TArray<FName> FriendlyFireTags;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Base Weapon")
		float RoundsPerMinute = 60;

	/** Projectile Spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Weapon")
		class USceneComponent* ProjectileSpawn;

	// Sets default values for this actor's properties
	ABaseWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Base Weapon")
		virtual void FireTrigger();

	UFUNCTION(BlueprintCallable, Category = "Base Weapon")
		virtual void ReleaseFireTrigger();

	UFUNCTION(BlueprintCallable, Category = "Base Weapon")
		virtual void FireWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Base Weapon")
		void WeaponFired();
	
	virtual void WeaponFired_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Base Weapon")
		virtual float GetWeaponCooldown();

	UFUNCTION(BlueprintCallable, Category = "Base Weapon")
		virtual float GetElapsedWeaponCooldown();

	UFUNCTION(BlueprintCallable, Category = "Base Weapon")
		virtual float GetRemainingWeaponCooldown();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bFireTrigger = false;

	bool bWeaponOnCooldown = false;

	virtual void WeaponOffCooldown();

	FTimerHandle WeaponCooldownHandle;
private:

};

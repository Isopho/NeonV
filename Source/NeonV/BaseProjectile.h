// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class NEONV_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// ======================================================================================
	//	public Properties
	// ======================================================================================

	/** Projectile Movement Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Projectile")
		class UProjectileMovementComponent* ProjectileMovement;

	/** Projectile Collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Projectile")
		class UShapeComponent* ProjectileCollision;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Base Projectile")
		TArray<FName> IgnoreCollisionTags;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Base Projectile")
		float Damage = 1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base Projectile")
		FName ProjectileTag = FName("Projectile");

	// ======================================================================================
	//	Collision Functions
	// ======================================================================================

	/** Delegate for Projectile's overlap begin event. */
	UFUNCTION()
		void OnProjectileBeginOverlap
		(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep, const
			FHitResult& SweepResult
		);

	/** Delegate for Projectile's overlap end event. */
	UFUNCTION()
		void OnProjectileEndOverlap
		(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);

	UFUNCTION(BlueprintImplementableEvent, Category = "Base Projectile")
		void OnProjectileCollision(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep, const
			FHitResult& SweepResult);


	// ======================================================================================
	//	General Functions
	// ======================================================================================

	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

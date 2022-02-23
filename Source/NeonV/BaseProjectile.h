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
	/** Projectile Movement Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Projectile")
		class UProjectileMovementComponent* ProjectileMovement;

	/** Projectile Collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Projectile")
		class UShapeComponent* ProjectileCollision;

	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

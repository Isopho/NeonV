// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEONV_API ABaseAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base AI Character")
		FVector LookAtLocation = FVector::ZeroVector;

protected:

	virtual FVector CalculateDesiredOrientation() override;

	virtual void TurnCharacter(float AngleToDesiredOrientation) override;
};

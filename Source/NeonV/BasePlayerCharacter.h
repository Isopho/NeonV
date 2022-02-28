// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BasePlayerCharacter.generated.h"


UENUM(BlueprintType)
enum class EAimingMethod : uint8
{
	ControllerAiming    UMETA(DisplayName = "ControllerAiming"),
	MouseAiming			UMETA(DisplayName = "MouseAiming"),
	None				UMETA(DisplayName = "None"),
};

/**
 * 
 */
UCLASS()
class NEONV_API ABasePlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	// ======================================================================================
	//	public Properties
	// ======================================================================================

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Base Player Character")
		FName CharacterCenterTag = FName("CharacterCenter");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Player Character")
		EAimingMethod PlayerAimingMethod = EAimingMethod::MouseAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Player Character")
		float BaseTurnRate = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Player Character")
		float SoftTurnRadian = 30;

	/* Experimental	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Player Character")
		bool ThumbstickBasedFiringEnabled = false; 

	/* Experimental	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Player Character")
		float ThumbstickBasedFiringArea = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Base Player Character")
		int PlayerIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Player Character")
		float RightThumbstickDeadZone = 0.5;

	// ======================================================================================
	//	General Functions
	// ======================================================================================
	ABasePlayerCharacter();

	virtual void Tick(float DeltaTime) override;		

	/* Experimental	*/
	UFUNCTION(BlueprintCallable, Category = "Base Player Character")
		virtual bool DetermineThumbstickBasedFiring(float AngleToDesiredOrientation);


protected:

	// ======================================================================================
	//	protected Properties
	// ======================================================================================

	USceneComponent* CharacterCenterComponent;

	FVector2D ThumbstickOrientation;

	// ======================================================================================
	//	General Functions
	// ======================================================================================

	virtual void BeginPlay() override;

	// ======================================================================================
	//	Input & Character Controls
	// ======================================================================================

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void KeyboardAction();
	virtual void MouseAction();
	virtual void GamepadAction();

	// --------------------------------------------------------------------------------------
	//	Character Aiming
	// --------------------------------------------------------------------------------------
	void FaceForward(float Value);
	void FaceRight(float Value);

	void MoveForward(float Value);
	void MoveRight(float Value);

	virtual void AdjustCharacterOrientation();

	virtual FVector CalculateDesiredOrientation();

	virtual void TurnCharacter(float AngleToDesiredOrientation);

private:
};

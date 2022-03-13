// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Player")); // Did not work in contructor(ABasePlayerCharacter::ABasePlayerCharacter) for some reason
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ======================================================================================
//	Input & Character Controls
// ======================================================================================

void ABasePlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	InputComponent->BindAction("KeyboardAction", IE_Pressed, this, &ABasePlayerCharacter::KeyboardAction);
	InputComponent->BindAction("MouseAction", IE_Pressed, this, &ABasePlayerCharacter::MouseAction);
	InputComponent->BindAction("GamepadAction", IE_Pressed, this, &ABasePlayerCharacter::GamepadAction);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("FaceForward", this, &ABasePlayerCharacter::FaceForward);
	PlayerInputComponent->BindAxis("FaceRight", this, &ABasePlayerCharacter::FaceRight);


	InputComponent->BindAction("Fire", IE_Pressed, this, &ABasePlayerCharacter::FireTrigger);
	InputComponent->BindAction("Fire", IE_Released, this, &ABasePlayerCharacter::ReleaseFireTrigger);
}

void ABasePlayerCharacter::KeyboardAction()
{
	MouseAction();
}

void ABasePlayerCharacter::MouseAction()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if (PC)
	{
		PC->bShowMouseCursor = true;
	}

	PlayerAimingMethod = EAimingMethod::MouseAiming;
}

void ABasePlayerCharacter::GamepadAction()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if (PC)
	{
		PC->bShowMouseCursor = false;
	}

	PlayerAimingMethod = EAimingMethod::ControllerAiming;
}


// --------------------------------------------------------------------------------------
//	Character Aiming
// --------------------------------------------------------------------------------------

void ABasePlayerCharacter::FaceForward(float Value)
{
	ThumbstickOrientation.Y = -Value;
}

void ABasePlayerCharacter::FaceRight(float Value)
{
	ThumbstickOrientation.X = Value;	
}


FVector ABasePlayerCharacter::CalculateDesiredOrientation()
{
	// Fallback value is the characters current orientation.
	FVector DesiredOrientation = CharacterCenterComponent->GetForwardVector();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"), *FString("No PlayerController found for Player " + PlayerIndex));
		return DesiredOrientation.GetSafeNormal(0.0001f);
	}

	switch (PlayerAimingMethod)
	{
	case EAimingMethod::ControllerAiming:
	{
		// --------------------------------------------------------------------------------------
		//	ControllerAiming
		// --------------------------------------------------------------------------------------
		if (ThumbstickOrientation.Size() >= RightThumbstickDeadZone)
		{
			FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();
			CamRotation.Pitch = 0;
			CamRotation.Roll = 0;

			FVector Forward = UKismetMathLibrary::GetForwardVector(CamRotation);
			FVector Right = UKismetMathLibrary::GetRightVector(CamRotation);

			DesiredOrientation = (Forward * ThumbstickOrientation.Y) + (Right * ThumbstickOrientation.X);
		}
		break;
		// --------------------------------------------------------------------------------------
	}
	case EAimingMethod::MouseAiming:
	{
		// --------------------------------------------------------------------------------------
		//	MouseAiming
		// --------------------------------------------------------------------------------------
		FVector MouseLocation, MouseDirection;
		bool MouseDeprojectionSuccessful = PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection);

		if (MouseDeprojectionSuccessful) {
			FVector CharacterCenterLocation = CharacterCenterComponent->GetComponentLocation();

			// Mouse projected onto a Z-plane at the CharacterCenter
			FVector MouseAimLocation = MouseLocation + (MouseDirection * ((CharacterCenterLocation.Z - MouseLocation.Z) / MouseDirection.Z));

			DesiredOrientation = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(CharacterCenterLocation, MouseAimLocation));
		}
		break;
		// --------------------------------------------------------------------------------------
	}
	default:
		break;
	}

	return DesiredOrientation.GetSafeNormal(0.0001f);
}

void ABasePlayerCharacter::TurnCharacter(float AngleToDesiredOrientation)
{
	// Reduce turnrate for "small" angles. (Angles within the SoftTurnRadian)
	float SoftTurnRateMultiplier = UKismetMathLibrary::FClamp(AngleToDesiredOrientation / SoftTurnRadian, -1.0f, 1.0f);

	AddControllerYawInput(SoftTurnRateMultiplier * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

// --------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------
//	Character Movement
// --------------------------------------------------------------------------------------
void ABasePlayerCharacter::MoveForward(float Value)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if (PC)
	{
		FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();
		CamRotation.Pitch = 0;
		CamRotation.Roll = 0;

		FVector Forward = UKismetMathLibrary::GetForwardVector(CamRotation);

		AddMovementInput(Forward, Value);
	}
}

void ABasePlayerCharacter::MoveRight(float Value)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	if (PC)
	{
		FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();
		CamRotation.Pitch = 0;
		CamRotation.Roll = 0;

		FVector Right = UKismetMathLibrary::GetRightVector(CamRotation);

		AddMovementInput(Right, Value);
	}
}



/* Experimental	*/
bool ABasePlayerCharacter::DetermineThumbstickBasedFiring(float AngleToDesiredOrientation)
{
	bool ShouldFire = false;
	if (ThumbstickOrientation.Size() >= RightThumbstickDeadZone && AngleToDesiredOrientation <= ThumbstickBasedFiringArea)
	{
		ShouldFire = true;
	}
	return ShouldFire;
}
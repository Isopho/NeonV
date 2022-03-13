// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "Kismet/KismetMathLibrary.h"



FVector ABaseAICharacter::CalculateDesiredOrientation()
{
	FVector DesiredOrientation;
	if (!LookAtLocation.IsZero()) {
		FVector CenterLocation = CharacterCenterComponent->GetComponentLocation();
		DesiredOrientation = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(CenterLocation, LookAtLocation));
	}
	else {
		DesiredOrientation = CharacterCenterComponent->GetForwardVector();
	}

	// use DesiredOrientation z-plane projection (?)
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString("ABaseAICharacter::CalculateDesiredOrientation " + DesiredOrientation.ToString()));
	return DesiredOrientation.GetSafeNormal(0.0001f);
}

void ABaseAICharacter::TurnCharacter(float AngleToDesiredOrientation)
{
	//if (UKismetMathLibrary::Abs(AngleToDesiredOrientation) >= 10) {
	//	AddActorWorldRotation(FRotator(0, AngleToDesiredOrientation/2, 0));
	//}

	//// Reduce turnrate for "small" angles. (Angles within the SoftTurnRadian)
	//float SoftTurnRateMultiplier = (UKismetMathLibrary::FClamp(AngleToDesiredOrientation / SoftTurnRadian, -1.0f, 1.0f) +1)/2 ;
	////SoftTurnRateMultiplier = UKismetMathLibrary::Lerp(SoftTurnRateMultiplier, 0, UKismetMathLibrary::Abs(SoftTurnRateMultiplier));

	//
	//float yaw = UKismetMathLibrary::Lerp(BaseTurnRate, -BaseTurnRate, SoftTurnRateMultiplier) * GetWorld()->GetDeltaSeconds();

	////float yaw = SoftTurnRateMultiplier * BaseTurnRate * GetWorld()->GetDeltaSeconds();
	//UE_LOG(LogTemp, Error, TEXT("%s"), *FString("ABaseAICharacter::TurnCharacter " + FString::SanitizeFloat(yaw)));
	////AddControllerYawInput(SoftTurnRateMultiplier * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	//FRotator rotation = FRotator(0, yaw, 0);
	//AddActorLocalRotation(rotation);


	if (!LookAtLocation.IsZero()) {
		FVector CenterLocation = CharacterCenterComponent->GetComponentLocation();
		FRotator CenterRotation = FRotator(0, CharacterCenterComponent->GetComponentRotation().Yaw, 0);
		FRotator TargetRotation = FRotator(0, UKismetMathLibrary::FindLookAtRotation(CenterLocation, LookAtLocation).Yaw, 0);

		FRotator DeltaToDesiredRotation = UKismetMathLibrary::NormalizedDeltaRotator(TargetRotation , CenterRotation);
		//float TurnAngle = BaseTurnRate * GetWorld()->GetDeltaSeconds() * UKismetMathLibrary::FClamp(DeltaToDesiredRotation.Yaw, -1.0f, 1.0f);
		float TurnAngle = UKismetMathLibrary::FClamp(DeltaToDesiredRotation.Yaw, -1 * BaseTurnRate * GetWorld()->GetDeltaSeconds(), BaseTurnRate * GetWorld()->GetDeltaSeconds());

		UE_LOG(LogTemp, Error, TEXT("%s"), *FString("ABaseAICharacter::TurnCharacter TurnAngle " + FString::SanitizeFloat(TurnAngle)));
		FRotator HardRotation = FRotator(0, CenterRotation.Yaw + TurnAngle, 0);
		
		//float AbsAngleToDesiredOrientation1 = UKismetMathLibrary::FClamp(UKismetMathLibrary::Abs(DeltaToDesiredRotation.Yaw), 0 , SoftTurnRadian);

		//float SoftTurnRate = UKismetMathLibrary::Abs(UKismetMathLibrary::FClamp(DeltaToDesiredRotation.Yaw / SoftTurnRadian, -1.0f, 1.0f));

		FRotator SoftRotation = UKismetMathLibrary::RLerp(CenterRotation, HardRotation, 0.9f, true);
		//Rotation = F
		SetActorRotation(SoftRotation);
		//this->SetWorldRotation(Rotation);
		//AddActorWorldRotation(Rotation);



		//AddControllerYawInput(SoftTurnRateMultiplier * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}
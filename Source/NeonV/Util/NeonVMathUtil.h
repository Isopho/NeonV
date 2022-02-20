#pragma once

#include "Kismet/KismetMathLibrary.h"

namespace NeonVMath {

	/*
	* Calculates the angle between two vectors on a plane given by its normal.
	*/
	float CalculateAngleOnPlane(FVector Vector1, FVector Vector2, FVector PlaneNormal) {
		float Angle;

		UKismetMathLibrary::Vector_Normalize(Vector1, 0.0001);
		UKismetMathLibrary::Vector_Normalize(Vector2, 0.0001);

		FVector Vector1h = UKismetMathLibrary::ProjectVectorOnToPlane(Vector1, PlaneNormal);
		FVector Vector2h = UKismetMathLibrary::ProjectVectorOnToPlane(Vector2, PlaneNormal);

		Angle = UKismetMathLibrary::DegAcos(UKismetMathLibrary::Dot_VectorVector(Vector1h, Vector2h));

		// Calculate if the angle is negative
		if (UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Cross_VectorVector(Vector1h, Vector2h), PlaneNormal) < 0)
		{
			Angle *= -1;
		}

		return Angle;

	}

	/*
	* Calculates the horizontal (x,y-Axis) angle between two vectors.
	*/
	float CalculateHorizontalAngle(FVector vector1, FVector vector2) {
		return CalculateAngleOnPlane(vector1, vector2, UKismetMathLibrary::Vector_Up());
	}

}
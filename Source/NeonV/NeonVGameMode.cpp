// Copyright Epic Games, Inc. All Rights Reserved.

#include "NeonVGameMode.h"
#include "NeonVCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANeonVGameMode::ANeonVGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

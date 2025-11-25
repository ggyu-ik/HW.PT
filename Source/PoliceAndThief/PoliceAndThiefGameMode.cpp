// Copyright Epic Games, Inc. All Rights Reserved.

#include "PoliceAndThiefGameMode.h"
#include "PoliceAndThiefCharacter.h"
#include "UObject/ConstructorHelpers.h"

APoliceAndThiefGameMode::APoliceAndThiefGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

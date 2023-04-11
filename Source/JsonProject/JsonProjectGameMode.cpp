// Copyright Epic Games, Inc. All Rights Reserved.

#include "JsonProjectGameMode.h"
#include "JsonProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJsonProjectGameMode::AJsonProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

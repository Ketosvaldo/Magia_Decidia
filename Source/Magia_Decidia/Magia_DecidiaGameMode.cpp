// Copyright Epic Games, Inc. All Rights Reserved.

#include "Magia_DecidiaGameMode.h"
#include "Magia_DecidiaCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMagia_DecidiaGameMode::AMagia_DecidiaGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

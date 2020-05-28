// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "UE4CPPExampleGameMode.h"
#include "UE4CPPExampleHUD.h"
#include "UE4CPPExampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE4CPPExampleGameMode::AUE4CPPExampleGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUE4CPPExampleHUD::StaticClass();
}

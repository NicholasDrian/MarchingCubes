// Copyright Epic Games, Inc. All Rights Reserved.


#include "MarchingCubesGameModeBase.h"
#include "Tests.h"
#include "DebugPawn.h"

AMarchingCubesGameModeBase::AMarchingCubesGameModeBase()
{
	//DefaultPawnClass = ADebugPawn::StaticClass();
	Tests::RunTests();
}

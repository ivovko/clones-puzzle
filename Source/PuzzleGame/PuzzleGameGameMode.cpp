// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleGameGameMode.h"
#include "Kismet/GameplayStatics.h"

APuzzleGameGameMode::APuzzleGameGameMode()
{
	// stub
}

void APuzzleGameGameMode::RestartCurrentLevel()
{
	if (UWorld* World = GetWorld())
	{
		UGameplayStatics::OpenLevel(World, FName(*UGameplayStatics::GetCurrentLevelName(World, true)));
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.


#include "PuzzleGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PuzzleGameCameraManager.h"
#include "PuzzleGameGameMode.h"

APuzzleGamePlayerController::APuzzleGamePlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = APuzzleGameCameraManager::StaticClass();
}

void APuzzleGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}

	CurrentInputComponent = InputComponent;
	OnInputComponentReady.Broadcast(InputComponent);
}

void APuzzleGamePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(PossessedPawn))
	{
		PossessedPawn->OnDestroyed.RemoveDynamic(this, &APuzzleGamePlayerController::HandlePossessedPawnDestroyed);
	}

	PossessedPawn = InPawn;
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->OnDestroyed.AddUniqueDynamic(this, &APuzzleGamePlayerController::HandlePossessedPawnDestroyed);
	}

}

void APuzzleGamePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(PossessedPawn))
	{
		PossessedPawn->OnDestroyed.RemoveDynamic(this, &APuzzleGamePlayerController::HandlePossessedPawnDestroyed);
	}

	Super::EndPlay(EndPlayReason);
}

void APuzzleGamePlayerController::HandlePossessedPawnDestroyed(AActor* DestroyedActor)
{
	if (DestroyedActor == PossessedPawn)
	{
		if (UWorld* World = GetWorld())
		{
			if (World->bIsTearingDown)
			{
				return;
			}

			if (APuzzleGameGameMode* GameMode = World->GetAuthGameMode<APuzzleGameGameMode>())
			{
				GameMode->RestartCurrentLevel();
			}
		}
	}
}

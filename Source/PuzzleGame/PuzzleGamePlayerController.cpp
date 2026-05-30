// Copyright Epic Games, Inc. All Rights Reserved.


#include "PuzzleGamePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PuzzleGameCameraManager.h"
#include "CloneSystem/InputBindableComponent.h"

APuzzleGamePlayerController::APuzzleGamePlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = APuzzleGameCameraManager::StaticClass();
	CloneAbility = CreateDefaultSubobject<UCloneAbilityComponent>(TEXT("CloneAbilityComponent"));
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

	IInputBindableComponent::Execute_SetupInputComponent(CloneAbility, InputComponent);
}

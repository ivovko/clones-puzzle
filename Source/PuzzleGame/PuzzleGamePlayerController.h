// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Mechanics/InputComponentProviderInterface.h"
#include "PuzzleGamePlayerController.generated.h"

class UInputMappingContext;
class UInputComponent;
class APawn;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract)
class PUZZLEGAME_API APuzzleGamePlayerController : public APlayerController, public IInputComponentProviderInterface
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<APawn> PossessedPawn;

	UPROPERTY(Transient)
	TObjectPtr<UInputComponent> CurrentInputComponent;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void HandlePossessedPawnDestroyed(AActor* DestroyedActor);
	
public:

	/** Constructor */
	APuzzleGamePlayerController();

	UPROPERTY(BlueprintAssignable, Category = "Input")
	FInputComponentReadyEvent OnInputComponentReady;

	virtual FInputComponentReadyEvent& GetInputComponentReadyEvent() override { return OnInputComponentReady; }
	virtual UInputComponent* GetCurrentInputComponent() const override { return CurrentInputComponent; }

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

};

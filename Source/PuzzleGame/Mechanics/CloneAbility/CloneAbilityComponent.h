#pragma once

#include "CoreMinimal.h"
#include "InputBindableInterface.h"
#include "CloneableCharacter.h"
#include "CloneAbilityComponent.generated.h"

class UInputAction;

/*
	This component is responsible for handling clone spawning and possession.
	It should be attached to the player controller. 

	This component expects that controlled pawn is of type ACloneableCharacter,
	and the player controller to map the input actions to the ones specified in the component.
*/

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UCloneAbilityComponent : public UActorComponent, public IInputBindableInterface
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SpawnClone();

	void SlowTime();

	void PossessClone();

	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	UPROPERTY()
	TObjectPtr<ACloneableCharacter> CurrentClone;

	UPROPERTY()
	TObjectPtr<ACloneableCharacter> PointedClone;

	UPROPERTY()
	TSubclassOf<ACloneableCharacter> ClonePawnClass;

	UPROPERTY()
	TArray<TObjectPtr<ACloneableCharacter>> Clones;

public:
	UCloneAbilityComponent();

	virtual void SetupInputComponent(UInputComponent *InputComponent) override;

	void OnPossessClone(ACloneableCharacter *Clone);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> SpawnCloneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> PossessionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PossessionRange = 100000.0f;

};

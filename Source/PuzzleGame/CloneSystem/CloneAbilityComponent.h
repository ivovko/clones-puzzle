#pragma once

#include "CoreMinimal.h"
#include "InputBindableComponent.h"
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
class UCloneAbilityComponent : public UActorComponent, public IInputBindableComponent
{
	GENERATED_BODY()
	
	virtual void SetupInputComponent_Implementation(UInputComponent *InputComponent) override;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SpawnClone();

	void SlowTime();

	void PossessClone();

	UFUNCTION(BlueprintCallable)
	void OnPossessClone(ACloneableCharacter *Clone);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SpawnCloneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> PossessionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PossessionRange = 100000.0f;

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

};

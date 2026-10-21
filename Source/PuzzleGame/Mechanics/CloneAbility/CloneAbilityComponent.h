#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloneAbilityComponent.generated.h"

class ACharacter;
class APawn;
class APlayerController;
class UEnhancedInputComponent;
class UInputAction;
class UInputComponent;
class UClonePawnComponent;

/*
	This component manages clone spawning and possession for ACharacter pawns
	that have a UClonePawnComponent.
*/

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UCloneAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void SpawnClone();

	void EvictOldestNonCurrentClone();

	void SlowTime();

	void RestoreTimeDilation();

	void PossessClone();

	void SetPointedClone(ACharacter* NewPointedClone);

	UFUNCTION()
	void HandleInputComponentReady(UInputComponent* InputComponent);

	UFUNCTION()
	void HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	TWeakObjectPtr<UEnhancedInputComponent> BoundInputComponent;

	UPROPERTY()
	TObjectPtr<ACharacter> CurrentClone;

	UPROPERTY()
	TObjectPtr<ACharacter> PointedClone;

	UPROPERTY()
	TSubclassOf<ACharacter> ClonePawnClass;

	UPROPERTY()
	TArray<TObjectPtr<ACharacter>> Clones;

public:
	UCloneAbilityComponent();

	UFUNCTION(BlueprintPure, Category = "Clone Ability")
	int32 GetCurrentCloneIndex() const;

	UFUNCTION(BlueprintPure, Category = "Clone Ability")
	int32 GetCurrentCloneCount() const { return Clones.Num(); }

	UFUNCTION(BlueprintPure, Category = "Clone Ability")
	bool IsCloneBlockActive(int32 BlockIndex) const { return BlockIndex == GetCurrentCloneIndex(); }

	UFUNCTION(BlueprintPure, Category = "Clone Ability")
	bool IsCloneBlockVisible(int32 BlockIndex) const { return BlockIndex < GetCurrentCloneCount(); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> SpawnCloneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> PossessionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PossessionRange = 100000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clone Ability", meta = (ClampMin = "2"))
	int32 MaxCloneCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clone Ability", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float AimTimeDilation = 0.3f;

};

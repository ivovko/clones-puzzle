#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClonePawnComponent.generated.h"

UCLASS(ClassGroup = (Mechanics), meta = (BlueprintSpawnableComponent))
class PUZZLEGAME_API UClonePawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Clone Ability")
	void SetSavedViewRotation(FRotator NewRotation);

	UFUNCTION(BlueprintPure, Category = "Clone Ability")
	FRotator GetSavedViewRotation() const;

private:
	UPROPERTY(Transient)
	FRotator SavedViewRotation = FRotator::ZeroRotator;
};

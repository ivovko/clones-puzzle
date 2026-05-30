#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../ViewInteractionSystem/ViewInteractableInterface.h"
#include "../ViewInteractionSystem/ViewInteractionComponent.h"
#include "CloneableCharacter.generated.h"

/*
    This class provides the functionality for characters that
    can be cloned under usage of the clone ability system.
*/
UCLASS(BlueprintType)
class ACloneableCharacter : public ACharacter, public IViewInteractableInterface
{
    GENERATED_BODY()

    FRotator CameraRotation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UViewInteractionComponent> ViewInteraction;

public:
    ACloneableCharacter();

    UFUNCTION(BlueprintCallable)
    UViewInteractionComponent *GetViewInteraction() const { return ViewInteraction; }

    UFUNCTION(BlueprintCallable)
    void SetCameraRotation(FRotator NewCameraRotation) { CameraRotation = NewCameraRotation; }

    UFUNCTION(BlueprintCallable)
    FRotator GetCameraRotation() const { return CameraRotation; }

};
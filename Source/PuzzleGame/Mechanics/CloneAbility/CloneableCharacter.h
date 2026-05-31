#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../ViewInteraction/ViewInteractableInterface.h"
#include "../ViewInteraction/ViewInteractionComponent.h"
#include "CloneableCharacter.generated.h"

/*
    This class provides the functionality for characters that
    can be cloned under usage of the clone ability.
*/
UCLASS(BlueprintType)
class ACloneableCharacter : public ACharacter, public IViewInteractableInterface
{
    GENERATED_BODY()

    FRotator CameraRotation;

public:
    ACloneableCharacter();

    UFUNCTION(BlueprintCallable)
    UViewInteractionComponent *GetViewInteraction() const { return ViewInteraction; }

    UFUNCTION(BlueprintCallable)
    void SetCameraRotation(FRotator NewCameraRotation) { CameraRotation = NewCameraRotation; }

    UFUNCTION(BlueprintCallable)
    FRotator GetCameraRotation() const { return CameraRotation; }

    virtual UMeshComponent *GetViewInteractableMesh() const override { return GetMesh(); }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UViewInteractionComponent> ViewInteraction;
};
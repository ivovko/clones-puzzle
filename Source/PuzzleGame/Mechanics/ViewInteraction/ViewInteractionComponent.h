#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ViewInteractionComponent.generated.h"

/*
    This component is responsible for handling the visual feedback
    of interactable objects when they are pointed at by the player.

    This component requires its owner to implement the
    ViewInteractableInterface to function properly.
*/
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UViewInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

    TObjectPtr<UMeshComponent> OwnerMesh;
    
    TArray<TObjectPtr<UMaterialInterface>> OriginalMaterials;

    int32 NumMaterials = 0;

    virtual void BeginPlay() override;

public:
    UViewInteractionComponent();

    UFUNCTION(BlueprintCallable)
    void StartPointed();

    UFUNCTION(BlueprintCallable)
    void EndPointed();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UPrimitiveComponent> CollisionSource;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<UMaterialInterface>> HighlightMaterials;

};
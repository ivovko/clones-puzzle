#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ViewInteractableComponent.generated.h"

/*
    This component is responsible for handling the visual feedback
    of interactable objects when they are pointed at by the player.

    This component requires its owner to implement the
    ViewInteractableInterface to function properly.
*/
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UViewInteractableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UViewInteractableComponent();

    virtual void BeginPlay() override;

    int32 NumMaterials = 0;

    UFUNCTION(BlueprintCallable)
    void StartPointed();

    UFUNCTION(BlueprintCallable)
    void EndPointed();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UPrimitiveComponent* CollisionSource;

    UPROPERTY()
    UMeshComponent* OwnerMesh;
    
    UPROPERTY()
    TArray<UMaterialInterface*> OriginalMaterials;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UMaterialInterface*> HighlightMaterials;

};
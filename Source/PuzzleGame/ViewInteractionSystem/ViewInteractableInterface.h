#pragma once

#include "CoreMinimal.h"
#include "ViewInteractableInterface.generated.h"

/*
    This interface defines the contract for objects that can be interacted with visually.
    
    This interface is required by the ViewInteractableComponent to function properly.
*/
UINTERFACE(BlueprintType)
class UViewInteractableInterface: public UInterface
{
    GENERATED_BODY()
};

class IViewInteractableInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UMeshComponent* GetViewInteractableMesh();
};
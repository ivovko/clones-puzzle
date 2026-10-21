#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ViewInteractableInterface.generated.h"

class UMeshComponent;

/*
    This interface defines the contract for objects that can be interacted with visually.
    
    This interface is required by the ViewInteractionComponent to function properly.
*/
UINTERFACE(BlueprintType, Blueprintable)
class UViewInteractableInterface: public UInterface
{
    GENERATED_BODY()
};

class IViewInteractableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    UMeshComponent* GetViewInteractableMesh() const;

};
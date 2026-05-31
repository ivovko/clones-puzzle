#pragma once

#include "CoreMinimal.h"
#include "InputBindableInterface.generated.h"


UINTERFACE(BlueprintType)
class UInputBindableInterface: public UInterface
{
    GENERATED_BODY()
};

class IInputBindableInterface
{
    GENERATED_BODY()

public:
    virtual void SetupInputComponent(UInputComponent* InputComponent) = 0;

};
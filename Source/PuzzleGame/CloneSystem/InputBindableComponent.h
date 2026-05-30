#pragma once

#include "CoreMinimal.h"
#include "InputBindableComponent.generated.h"


UINTERFACE(BlueprintType)
class UInputBindableComponent: public UInterface
{
    GENERATED_BODY()
};

class IInputBindableComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SetupInputComponent(UInputComponent* InputComponent);

};
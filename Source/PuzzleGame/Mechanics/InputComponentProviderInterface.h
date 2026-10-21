#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputComponentProviderInterface.generated.h"

class UInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputComponentReadyEvent, UInputComponent*, InputComponent);

UINTERFACE()
class UInputComponentProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class IInputComponentProviderInterface
{
	GENERATED_BODY()

public:
	virtual FInputComponentReadyEvent& GetInputComponentReadyEvent() = 0;
	virtual UInputComponent* GetCurrentInputComponent() const = 0;
};
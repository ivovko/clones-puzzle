#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DataFlowInteractableInterface.generated.h"

UINTERFACE(Blueprintable)
class UDataFlowInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

class IDataFlowInteractableInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent)
    void OnFlowRedirectStart(class ALaserDataFlow* DataFlow);

    UFUNCTION(BlueprintNativeEvent)
    void OnFlowRedirectEnd(class ALaserDataFlow* DataFlow);
};

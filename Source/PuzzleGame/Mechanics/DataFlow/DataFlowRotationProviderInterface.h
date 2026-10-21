#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DataFlowRotationProviderInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UDataFlowRotationProviderInterface : public UInterface
{
	GENERATED_BODY()
};

class IDataFlowRotationProviderInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	FRotator GetDataFlowRotation() const;
};
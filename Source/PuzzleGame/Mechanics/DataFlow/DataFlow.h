#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DataFlowInteractableInterface.h"
#include "DataFlow.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ALaserDataFlow : public AActor
{
    GENERATED_BODY()

public:
    ALaserDataFlow();

    virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    virtual void Rerender(FVector location, FRotator rotation);

    UMeshComponent* GetDataFlowMesh() const { return DataFlowMesh; }

    void IgnoreActor(AActor* Actor) { actorsToIgnore.Add(Actor); }

private:
    UPROPERTY()
    TObjectPtr<UArrowComponent> DirectionArrow;

    TWeakObjectPtr<AActor> LastInteractable;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
    TArray<AActor*> actorsToIgnore;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMeshComponent* DataFlowMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxTraceDistance = 10000.0f;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataFlow.generated.h"

class UArrowComponent;
class UDataFlowInteractionComponent;
class UMeshComponent;
class USceneComponent;

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class ALaserDataFlow : public AActor
{
    GENERATED_BODY()

public:
    ALaserDataFlow();

    virtual void Tick(float DeltaTime) override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    virtual void Rerender(FVector location, FRotator rotation);

    void FollowComponents(USceneComponent* InLocationUpdateTrigger, USceneComponent* InRotationUpdateTrigger,
        AActor* InProvider);

    UMeshComponent* GetDataFlowMesh() const { return DataFlowMesh; }

    void IgnoreActor(AActor* Actor) { actorsToIgnore.Add(Actor); }

private:
    void HandleFollowedComponentTransformUpdated(USceneComponent* UpdatedComponent, EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport);
    void UpdateInteraction(UDataFlowInteractionComponent* HitInteractionComponent);
    void StopFollowingComponents();
    void UpdateFromFollowedComponents();

    UPROPERTY()
    TObjectPtr<UArrowComponent> DirectionArrow;

    TWeakObjectPtr<UDataFlowInteractionComponent> LastInteractionComponent;

    TWeakObjectPtr<USceneComponent> LocationUpdateTrigger;
    TWeakObjectPtr<USceneComponent> RotationUpdateTrigger;
    TWeakObjectPtr<AActor> Provider;
    FDelegateHandle LocationTransformUpdatedHandle;
    FDelegateHandle RotationTransformUpdatedHandle;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
    TArray<AActor*> actorsToIgnore;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UMeshComponent* DataFlowMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MaxTraceDistance = 10000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Flow", meta = (ClampMin = "1.0"))
    float DistancePerScaleUnit = 200.0f;
};

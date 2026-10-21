#include "DataFlow.h"
#include "DataFlowRotationProviderInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DataFlowInteractionComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../CollisionChannels.h"

namespace
{
    constexpr float DirectionArrowPitchOffsetDegrees = 90.0f;
}

ALaserDataFlow::ALaserDataFlow()
{
    PrimaryActorTick.bCanEverTick = true;
    DataFlowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DataFlowMesh"));
    SetRootComponent(DataFlowMesh);

    DirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));
    DirectionArrow->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    DirectionArrow->SetupAttachment(DataFlowMesh);
}

void ALaserDataFlow::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FHitResult Hit;

    UKismetSystemLibrary::LineTraceSingle(
        GetWorld(),
        DirectionArrow->GetComponentLocation(),
        DirectionArrow->GetComponentLocation() + DirectionArrow->GetForwardVector() * MaxTraceDistance,
        UEngineTypes::ConvertToTraceType(PuzzleGameCollisionChannels::Interaction),
        false,
        actorsToIgnore,
        EDrawDebugTrace::None,
        Hit,
        true);

    FVector tmp;

    if (Hit.bBlockingHit)
    {
        tmp = Hit.Location;
    }
    else
    {
        tmp = Hit.TraceEnd;
    }

    tmp = tmp - Hit.TraceStart;

    FVector CurrentScale = DataFlowMesh->GetComponentScale();
    DataFlowMesh->SetWorldScale3D(FVector(CurrentScale.X, CurrentScale.Y, tmp.Size() / DistancePerScaleUnit));

    AActor* HitActor = Hit.GetActor();
    UDataFlowInteractionComponent* HitInteractionComponent = IsValid(HitActor)
        ? HitActor->FindComponentByClass<UDataFlowInteractionComponent>()
        : nullptr;
    UpdateInteraction(HitInteractionComponent);
}

void ALaserDataFlow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UpdateInteraction(nullptr);
    StopFollowingComponents();
    Super::EndPlay(EndPlayReason);
}

void ALaserDataFlow::UpdateInteraction(UDataFlowInteractionComponent* HitInteractionComponent)
{
    if (LastInteractionComponent.Get() == HitInteractionComponent)
    {
        return;
    }

    if (UDataFlowInteractionComponent* PreviousComponent = LastInteractionComponent.Get())
    {
        PreviousComponent->EndInteraction(this);
    }

    LastInteractionComponent = HitInteractionComponent;

    if (IsValid(HitInteractionComponent))
    {
        HitInteractionComponent->StartInteraction(this);
    }
}

void ALaserDataFlow::FollowComponents(USceneComponent* InLocationUpdateTrigger, USceneComponent* InRotationUpdateTrigger,
    AActor* InProvider)
{
    StopFollowingComponents();

    if (!IsValid(InLocationUpdateTrigger) || !IsValid(InRotationUpdateTrigger) || !IsValid(InProvider) ||
        !InProvider->Implements<UDataFlowRotationProviderInterface>())
    {
        return;
    }

    LocationUpdateTrigger = InLocationUpdateTrigger;
    RotationUpdateTrigger = InRotationUpdateTrigger;
    Provider = InProvider;
    LocationTransformUpdatedHandle = InLocationUpdateTrigger->TransformUpdated.AddUObject(this, &ALaserDataFlow::HandleFollowedComponentTransformUpdated);

    if (InRotationUpdateTrigger != InLocationUpdateTrigger)
    {
        RotationTransformUpdatedHandle = InRotationUpdateTrigger->TransformUpdated.AddUObject(this, &ALaserDataFlow::HandleFollowedComponentTransformUpdated);
    }

    UpdateFromFollowedComponents();
}

void ALaserDataFlow::HandleFollowedComponentTransformUpdated(USceneComponent*, EUpdateTransformFlags, ETeleportType)
{
    UpdateFromFollowedComponents();
}

void ALaserDataFlow::StopFollowingComponents()
{
    if (USceneComponent* LocationTrigger = LocationUpdateTrigger.Get())
    {
        LocationTrigger->TransformUpdated.Remove(LocationTransformUpdatedHandle);
    }

    if (USceneComponent* RotationTrigger = RotationUpdateTrigger.Get())
    {
        RotationTrigger->TransformUpdated.Remove(RotationTransformUpdatedHandle);
    }

    LocationUpdateTrigger = nullptr;
    RotationUpdateTrigger = nullptr;
    Provider = nullptr;
    LocationTransformUpdatedHandle.Reset();
    RotationTransformUpdatedHandle.Reset();
}

void ALaserDataFlow::UpdateFromFollowedComponents()
{
    AActor* ResolvedProvider = Provider.Get();
    if (IsValid(ResolvedProvider))
    {
        const FRotator Rotation = IDataFlowRotationProviderInterface::Execute_GetDataFlowRotation(ResolvedProvider);
        Rerender(ResolvedProvider->GetActorLocation(), Rotation);
    }
}

void ALaserDataFlow::Rerender(FVector location, FRotator rotation)
{
    rotation.Pitch -= DirectionArrowPitchOffsetDegrees;

    SetActorLocation(location);
    SetActorRotation(rotation);
}

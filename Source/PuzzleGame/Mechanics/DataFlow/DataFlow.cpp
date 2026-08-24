#include "DataFlow.h"
#include "Kismet/KismetSystemLibrary.h"

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
        UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
        false,
        actorsToIgnore,
        EDrawDebugTrace::ForOneFrame,
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
    DataFlowMesh->SetWorldScale3D(FVector(CurrentScale.X, CurrentScale.Y, tmp.Size() / 200.0f));

    AActor* HitActor = Hit.GetActor();

    if (IsValid(HitActor) && HitActor->Implements<UDataFlowInteractableInterface>())
    {
        if (LastInteractable != HitActor)
        {
            if (LastInteractable.IsValid())
            {
                IDataFlowInteractableInterface::Execute_OnFlowRedirectEnd(LastInteractable.Get(), this);
            }

            LastInteractable = HitActor;
        }

        IDataFlowInteractableInterface::Execute_OnFlowRedirectStart(LastInteractable.Get(), this);
    }
    else if (LastInteractable.IsValid())
    {
        IDataFlowInteractableInterface::Execute_OnFlowRedirectEnd(LastInteractable.Get(), this);
        LastInteractable = nullptr;
    }
}

void ALaserDataFlow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    if (LastInteractable.IsValid())
    {
        IDataFlowInteractableInterface::Execute_OnFlowRedirectEnd(LastInteractable.Get(), this);
    }
}

void ALaserDataFlow::Rerender(FVector location, FRotator rotation)
{
    rotation.Pitch -= 90.0f;

    SetActorLocation(location);
    SetActorRotation(rotation);
}

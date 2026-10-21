#include "CloneAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "../InputComponentProviderInterface.h"
#include "ClonePawnComponent.h"
#include "../ViewInteraction/ViewInteractionComponent.h"
#include "../CollisionChannels.h"

UCloneAbilityComponent::UCloneAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCloneAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    PC = Cast<APlayerController>(GetOwner());
    if (IsValid(PC))
    {
        PC->OnPossessedPawnChanged.AddUniqueDynamic(this, &UCloneAbilityComponent::HandlePossessedPawnChanged);
        HandlePossessedPawnChanged(nullptr, PC->GetPawn());
    }
	if (IInputComponentProviderInterface* InputProvider = Cast<IInputComponentProviderInterface>(GetOwner()))
	{
		InputProvider->GetInputComponentReadyEvent().AddUniqueDynamic(this, &UCloneAbilityComponent::HandleInputComponentReady);
		HandleInputComponentReady(InputProvider->GetCurrentInputComponent());
	}
}

void UCloneAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (IsValid(PC))
    {
        PC->OnPossessedPawnChanged.RemoveDynamic(this, &UCloneAbilityComponent::HandlePossessedPawnChanged);
    }
    if (IInputComponentProviderInterface* InputProvider = Cast<IInputComponentProviderInterface>(GetOwner()))
    {
        InputProvider->GetInputComponentReadyEvent().RemoveDynamic(this, &UCloneAbilityComponent::HandleInputComponentReady);
    }
    if (UEnhancedInputComponent* InputComponent = BoundInputComponent.Get())
    {
        InputComponent->ClearBindingsForObject(this);
    }

    RestoreTimeDilation();

    Super::EndPlay(EndPlayReason);
}

void UCloneAbilityComponent::HandleInputComponentReady(UInputComponent* InputComponent)
{
    if (UEnhancedInputComponent* PreviouslyBoundInputComponent = BoundInputComponent.Get())
    {
        PreviouslyBoundInputComponent->ClearBindingsForObject(this);
    }
    BoundInputComponent = nullptr;

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
		BoundInputComponent = EnhancedInputComponent;
        EnhancedInputComponent->BindAction(SpawnCloneAction, ETriggerEvent::Started, this, &UCloneAbilityComponent::SpawnClone);
        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Started, this, &UCloneAbilityComponent::SlowTime);
        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Completed, this, &UCloneAbilityComponent::PossessClone);
        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Canceled, this, &UCloneAbilityComponent::RestoreTimeDilation);
    }
}

void UCloneAbilityComponent::SlowTime()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), AimTimeDilation);
}

void UCloneAbilityComponent::RestoreTimeDilation()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

void UCloneAbilityComponent::SpawnClone()
{
    if (!IsValid(CurrentClone) || !IsValid(PC) || !ClonePawnClass)
    {
        return;
    }

    ACharacter* NewClone = GetWorld()->SpawnActor<ACharacter>(ClonePawnClass, CurrentClone->GetActorTransform());

    if (NewClone)
    {
        NewClone->FindComponentByClass<UClonePawnComponent>()->SetSavedViewRotation(PC->GetControlRotation());

        if (Clones.Num() >= MaxCloneCount)
        {
            EvictOldestNonCurrentClone();
        }

        Clones.Add(NewClone);
    }
}

void UCloneAbilityComponent::EvictOldestNonCurrentClone()
{
    for (int32 Index = 0; Index < Clones.Num(); ++Index)
    {
        if (Clones[Index] != CurrentClone)
        {
            if (IsValid(Clones[Index]))
            {
                Clones[Index]->Destroy();
            }

            Clones.RemoveAt(Index);
            return;
        }
    }
}

void UCloneAbilityComponent::PossessClone()
{
    if (IsValid(PointedClone) && IsValid(CurrentClone) && IsValid(PC))
    {
        CurrentClone->FindComponentByClass<UClonePawnComponent>()->SetSavedViewRotation(PC->GetControlRotation());

        ACharacter* CloneToPossess = PointedClone;
        SetPointedClone(nullptr);
        PC->Possess(CloneToPossess);
        CurrentClone = CloneToPossess;
    }

    RestoreTimeDilation();
}

void UCloneAbilityComponent::SetPointedClone(ACharacter* NewPointedClone)
{
    if (PointedClone == NewPointedClone)
    {
        return;
    }

    if (IsValid(PointedClone))
    {
        if (UViewInteractionComponent* ViewComponent = PointedClone->FindComponentByClass<UViewInteractionComponent>())
        {
            ViewComponent->EndPointed();
        }
    }

    PointedClone = NewPointedClone;

    if (IsValid(PointedClone))
    {
        if (UViewInteractionComponent* ViewComponent = PointedClone->FindComponentByClass<UViewInteractionComponent>())
        {
            ViewComponent->StartPointed();
        }
    }
}

void UCloneAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsValid(CurrentClone))
    {
        if (PC->PlayerCameraManager)
        {
            FHitResult Hit;
            const FVector Start = PC->PlayerCameraManager->GetCameraLocation();
            const FVector End = Start + PC->PlayerCameraManager->GetCameraRotation().Vector() * PossessionRange;

            TArray<AActor *> ActorsToIgnore;
            ActorsToIgnore.Add(CurrentClone);

            const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(PuzzleGameCollisionChannels::Interaction);

            UKismetSystemLibrary::LineTraceSingle(
                GetWorld(),
                Start,
                End,
                TraceChannel,
                false,
                ActorsToIgnore,
                EDrawDebugTrace::None,
                Hit,
                true,
                FLinearColor::Red,
                FLinearColor::Green,
                5.0f);

            ACharacter* HitCharacter = Cast<ACharacter>(Hit.GetActor());
            if (IsValid(HitCharacter) && IsValid(HitCharacter->FindComponentByClass<UClonePawnComponent>()))
            {
                SetPointedClone(HitCharacter);
            }
            else
            {
                SetPointedClone(nullptr);
            }
        }
    }
}

void UCloneAbilityComponent::HandlePossessedPawnChanged(APawn*, APawn* NewPawn)
{
    ACharacter* Clone = Cast<ACharacter>(NewPawn);
    UClonePawnComponent* CloneComponent = IsValid(Clone) ? Clone->FindComponentByClass<UClonePawnComponent>() : nullptr;
    if (!IsValid(CloneComponent))
    {
        return;
    }

    if (IsValid(CurrentClone))
    {
        if (CurrentClone->GetCharacterMovement()->IsFalling() && Clone->GetCharacterMovement()->IsFalling())
        {
            Clone->LaunchCharacter(CurrentClone->GetVelocity(), true, true);
        }
    }
    else
    {
        ClonePawnClass = Clone->GetClass();
        Clones.Add(Clone);
    }

    CurrentClone = Clone;

    if (PC)
    {
        PC->SetControlRotation(CloneComponent->GetSavedViewRotation());
    }
}

int32 UCloneAbilityComponent::GetCurrentCloneIndex() const
{
    if (IsValid(CurrentClone))
    {
        return Clones.IndexOfByKey(CurrentClone);
    }
    return -1;
}

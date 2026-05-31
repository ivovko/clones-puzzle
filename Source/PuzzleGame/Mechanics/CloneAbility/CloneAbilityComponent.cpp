#include "CloneAbilityComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../ViewInteraction/ViewInteractionComponent.h"

UCloneAbilityComponent::UCloneAbilityComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UCloneAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    PC = Cast<APlayerController>(GetOwner());
    ClonePawnClass = CurrentClone->GetClass();
    Clones.Add(CurrentClone);
}

void UCloneAbilityComponent::SetupInputComponent(UInputComponent *InputComponent)
{
    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(SpawnCloneAction, ETriggerEvent::Started, this, &UCloneAbilityComponent::SpawnClone);
        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Started, this, &UCloneAbilityComponent::SlowTime);
        EnhancedInputComponent->BindAction(PossessionAction, ETriggerEvent::Completed, this, &UCloneAbilityComponent::PossessClone);
    }
}

void UCloneAbilityComponent::SlowTime()
{
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);
}

void UCloneAbilityComponent::SpawnClone()
{
    ACloneableCharacter *NewClone = GetWorld()->SpawnActor<ACloneableCharacter>(ClonePawnClass, CurrentClone->GetActorTransform());

    if (NewClone)
    {
        if (Clones.Num() == 3)
        {
            if (Clones[0] == CurrentClone)
            {
                Clones[1]->Destroy();
                Clones.RemoveAt(1);
            }
            else
            {
                Clones[0]->Destroy();
                Clones.RemoveAt(0);
            }
        }

        Clones.Add(NewClone);
    }
}

void UCloneAbilityComponent::PossessClone()
{
    if (PointedClone)
    {
        CurrentClone->SetCameraRotation(PC->GetControlRotation());
        PC->Possess(PointedClone);
        CurrentClone = PointedClone;
        PointedClone = nullptr;
    }

    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
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

            const ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2); // custom trace channel for clones, project specific

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

            if (Hit.GetActor() && Hit.GetActor()->IsA(ClonePawnClass))
            {
                if (PointedClone)
                {
                    if (PointedClone != Hit.GetActor())
                    {
                        PointedClone->GetViewInteraction()->EndPointed();
                        PointedClone = Cast<ACloneableCharacter>(Hit.GetActor());
                        PointedClone->GetViewInteraction()->StartPointed();
                    }
                }
                else
                {
                    PointedClone = Cast<ACloneableCharacter>(Hit.GetActor());
                    PointedClone->GetViewInteraction()->StartPointed();
                }
            }
            else if (PointedClone)
            {
                PointedClone->GetViewInteraction()->EndPointed();
                PointedClone = nullptr;
            }
        }
    }
    else
    {
        if (UWorld *World = GetWorld())
        {
            UGameplayStatics::OpenLevel(World, FName(*UGameplayStatics::GetCurrentLevelName(World, true)));
        }
    }
}

void UCloneAbilityComponent::OnPossessClone(ACloneableCharacter *Clone)
{
    if (IsValid(CurrentClone) && IsValid(Clone))
    {
        if (CurrentClone->GetCharacterMovement()->IsFalling() && Clone->GetCharacterMovement()->IsFalling())
        {
            Clone->LaunchCharacter(CurrentClone->GetVelocity(), true, true);
        }
    }

    CurrentClone = Clone;

    if (PC)
    {
        PC->SetControlRotation(CurrentClone->GetCameraRotation());
    }
}

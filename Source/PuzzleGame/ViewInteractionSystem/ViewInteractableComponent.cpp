#include "ViewInteractableComponent.h"
#include "ViewInteractableInterface.h" 
#include "PuzzleGameCharacter.h"
#include "Kismet/GameplayStatics.h"

   
UViewInteractableComponent::UViewInteractableComponent()
{
    //placeholder
}

void UViewInteractableComponent::BeginPlay()
{
    Super::BeginPlay();

    if (CollisionSource)
    {
        CollisionSource->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

    if (GetOwner()->GetClass()->ImplementsInterface(UViewInteractableInterface::StaticClass()))
    {
        OwnerMesh = IViewInteractableInterface::Execute_GetViewInteractableMesh(GetOwner());

        if (OwnerMesh)
        {
            NumMaterials = OwnerMesh->GetNumMaterials();

            OriginalMaterials.Reserve(NumMaterials);

            for (int32 i = 0; i < NumMaterials; i++)
            {
                OriginalMaterials.Add(OwnerMesh->GetMaterial(i));
            }
        }
    }
}

void UViewInteractableComponent::StartPointed()
{
    for (int32 i = 0; i < NumMaterials; i++)
    {
        UMaterialInterface* Material = nullptr;

        if (HighlightMaterials.IsValidIndex(i))
        {
            Material = HighlightMaterials[i];
        }

        if (Material)
        {
            OwnerMesh->SetMaterial(i, Material);
        }
    }
}

void UViewInteractableComponent::EndPointed()
{
    for (int32 i = 0; i < NumMaterials; i++)
    {
        UMaterialInterface* Material = nullptr;

        if (OriginalMaterials.IsValidIndex(i))
        {
            Material = OriginalMaterials[i];
        }

        if (Material)
        {
            OwnerMesh->SetMaterial(i, Material);
        }
    }
}
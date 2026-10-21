#include "ViewInteractionComponent.h"
#include "ViewInteractableInterface.h" 

void UViewInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (CollisionSource)
    {
        CollisionSource->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    }

    if (GetOwner()->Implements<UViewInteractableInterface>())
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

void UViewInteractionComponent::StartPointed()
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

void UViewInteractionComponent::EndPointed()
{
    for (int32 i = 0; i < NumMaterials; i++)
    {
        OwnerMesh->SetMaterial(i, OriginalMaterials[i]);
    }
}
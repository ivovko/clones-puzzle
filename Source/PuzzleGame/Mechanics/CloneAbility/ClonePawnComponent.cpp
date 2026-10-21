#include "ClonePawnComponent.h"

void UClonePawnComponent::SetSavedViewRotation(FRotator NewRotation)
{
	SavedViewRotation = NewRotation;
}

FRotator UClonePawnComponent::GetSavedViewRotation() const
{
	return SavedViewRotation;
}

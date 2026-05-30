#include "CloneableCharacter.h"

ACloneableCharacter::ACloneableCharacter()
{
    ViewInteraction = CreateDefaultSubobject<UViewInteractionComponent>(TEXT("ViewInteractionComponent"));
}
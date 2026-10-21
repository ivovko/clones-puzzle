#include "DataFlowInteractionComponent.h"
#include "DataFlow.h"
#include "DataFlowRotationProviderInterface.h"
#include "Components/SceneComponent.h"

void UDataFlowInteractionComponent::SetRotationUpdateSource(USceneComponent* InRotationUpdateSource)
{
	RotationUpdateSource.OverrideComponent = InRotationUpdateSource;
}

void UDataFlowInteractionComponent::SetRedirectsFlows(bool bShouldRedirect)
{
	if (bRedirectIncomingFlows == bShouldRedirect)
	{
		return;
	}

	bRedirectIncomingFlows = bShouldRedirect;
	if (!bRedirectIncomingFlows)
	{
		StopRedirect();
		return;
	}

	for (const TObjectPtr<ALaserDataFlow>& IncomingDataFlow : IncomingDataFlows)
	{
		if (IsValid(IncomingDataFlow))
		{
			StartRedirect(IncomingDataFlow);
			return;
		}
	}
}

void UDataFlowInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopRedirect();

	for (const TObjectPtr<ALaserDataFlow>& IncomingDataFlow : IncomingDataFlows.Array())
	{
		EndInteraction(IncomingDataFlow);
	}

	Super::EndPlay(EndPlayReason);
}

FTransform UDataFlowInteractionComponent::GetRedirectTransform(const ALaserDataFlow& IncomingDataFlow, AActor* Provider) const
{
	const FVector Location = Provider->GetActorLocation();
	const FVector Scale = IncomingDataFlow.GetDataFlowMesh()->GetComponentScale();
	const FRotator Rotation = IDataFlowRotationProviderInterface::Execute_GetDataFlowRotation(Provider);

	return FTransform(Rotation, Location, Scale);
}

void UDataFlowInteractionComponent::StartInteraction(ALaserDataFlow* IncomingDataFlow)
{
	if (!IsValid(IncomingDataFlow) || IncomingDataFlows.Contains(IncomingDataFlow))
	{
		return;
	}

	IncomingDataFlows.Add(IncomingDataFlow);
	OnFlowStarted.Broadcast(IncomingDataFlow);

	if (!IsValid(this) || !IsValid(IncomingDataFlow) || !IncomingDataFlows.Contains(IncomingDataFlow))
	{
		return;
	}

	if (bRedirectIncomingFlows)
	{
		StartRedirect(IncomingDataFlow);
	}
}

void UDataFlowInteractionComponent::EndInteraction(ALaserDataFlow* IncomingDataFlow)
{
	if (!IncomingDataFlows.Contains(IncomingDataFlow))
	{
		return;
	}

	IncomingDataFlows.Remove(IncomingDataFlow);
	OnFlowEnded.Broadcast(IncomingDataFlow);

	if (IncomingDataFlows.IsEmpty())
	{
		StopRedirect();
	}
}

void UDataFlowInteractionComponent::StartRedirect(ALaserDataFlow* IncomingDataFlow)
{
	AActor* Owner = GetOwner();
	USceneComponent* ResolvedRotationUpdateSource = Cast<USceneComponent>(RotationUpdateSource.GetComponent(Owner));
	if (IsValid(RedirectedDataFlow) || !IsValid(ResolvedRotationUpdateSource) ||
		!Owner->Implements<UDataFlowRotationProviderInterface>() || !IsValid(IncomingDataFlow->GetDataFlowMesh()))
	{
		return;
	}

	RedirectedDataFlow = GetWorld()->SpawnActor<ALaserDataFlow>(IncomingDataFlow->GetClass(), GetRedirectTransform(*IncomingDataFlow, Owner));
	if (IsValid(RedirectedDataFlow))
	{
		RedirectedDataFlow->IgnoreActor(Owner);
		RedirectedDataFlow->FollowComponents(Owner->GetRootComponent(), ResolvedRotationUpdateSource, Owner);
	}
}

void UDataFlowInteractionComponent::StopRedirect()
{
	if (IsValid(RedirectedDataFlow))
	{
		RedirectedDataFlow->Destroy();
		RedirectedDataFlow = nullptr;
	}
}
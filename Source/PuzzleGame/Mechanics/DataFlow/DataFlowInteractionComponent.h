#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "DataFlowInteractionComponent.generated.h"

class ALaserDataFlow;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDataFlowInteractionEvent, ALaserDataFlow*, DataFlow);

UCLASS(ClassGroup = (Mechanics), meta = (BlueprintSpawnableComponent))
class PUZZLEGAME_API UDataFlowInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Data Flow")
	void SetRotationUpdateSource(USceneComponent* InRotationUpdateSource);

	UFUNCTION(BlueprintCallable, Category = "Data Flow")
	void SetRedirectsFlows(bool bShouldRedirect);
	
	void StartInteraction(ALaserDataFlow* IncomingDataFlow);
	void EndInteraction(ALaserDataFlow* IncomingDataFlow);

	UPROPERTY(BlueprintAssignable, Category = "Data Flow")
	FDataFlowInteractionEvent OnFlowStarted;

	UPROPERTY(BlueprintAssignable, Category = "Data Flow")
	FDataFlowInteractionEvent OnFlowEnded;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	FTransform GetRedirectTransform(const ALaserDataFlow& IncomingDataFlow, AActor* Provider) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Flow", meta = (AllowPrivateAccess = "true"))
	FComponentReference RotationUpdateSource;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data Flow", meta = (AllowPrivateAccess = "true"))
	bool bRedirectIncomingFlows = false;

	UPROPERTY()
	TObjectPtr<ALaserDataFlow> RedirectedDataFlow;

	UPROPERTY()
	TSet<TObjectPtr<ALaserDataFlow>> IncomingDataFlows;

	void StartRedirect(ALaserDataFlow* IncomingDataFlow);
	void StopRedirect();
};
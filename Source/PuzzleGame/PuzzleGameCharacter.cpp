// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuzzleGameCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Mechanics/DataFlow/DataFlow.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APuzzleGameCharacter

APuzzleGameCharacter::APuzzleGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void APuzzleGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APuzzleGameCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APuzzleGameCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &APuzzleGameCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void APuzzleGameCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

	if (IsValid(this->RedirectedDataFlow))
	{
		this->RedirectedDataFlow->Rerender(GetActorLocation(), GetControlRotation());
	}

}

void APuzzleGameCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

	if (IsValid(this->RedirectedDataFlow))
	{
		this->RedirectedDataFlow->Rerender(GetActorLocation(), GetControlRotation());
	}	

}

void APuzzleGameCharacter::OnFlowRedirectStart_Implementation(ALaserDataFlow* DataFlow)
{
	IncomingDataFlows.Add(DataFlow);

	if (!IsValid(RedirectedDataFlow))
	{
		FRotator SpawnRotation = IsLocallyControlled() ? GetControlRotation() : GetCameraRotation();
		SpawnRotation.Pitch += -90.0f;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(GetCapsuleComponent()->GetComponentLocation());
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
		SpawnTransform.SetScale3D(DataFlow->GetDataFlowMesh()->GetComponentScale());

		ALaserDataFlow* SpawnedDataFlow = GetWorld()->SpawnActor<ALaserDataFlow>(DataFlow->GetClass(), SpawnTransform);

		if (SpawnedDataFlow)
		{
			SpawnedDataFlow->IgnoreActor(this);
		}

		RedirectedDataFlow = SpawnedDataFlow;
	}
}

void APuzzleGameCharacter::OnFlowRedirectEnd_Implementation(ALaserDataFlow* DataFlow)
{
	IncomingDataFlows.Remove(DataFlow);

	if (IncomingDataFlows.IsEmpty() && IsValid(RedirectedDataFlow))
	{
		RedirectedDataFlow->Destroy();
		RedirectedDataFlow = nullptr;
	}
}

void APuzzleGameCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void APuzzleGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void APuzzleGameCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void APuzzleGameCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void APuzzleGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(this->RedirectedDataFlow))
	{
		this->RedirectedDataFlow->Destroy();
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "MGP_2526Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MGP_2526.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Custom3DCMC.h"

//changes default sub-object of the CMC to custom CMC
AMGP_2526Character::AMGP_2526Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCustom3DCMC>(ACharacter::CharacterMovementComponentName))
{

	Custom3DCMC = Cast<UCustom3DCMC>(GetCharacterMovement());


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMGP_2526Character::BeginPlay() 
{
	Super::BeginPlay();

	defaultSocketOffset = CameraBoom->SocketOffset;
	defaultFieldOfView = FollowCamera->FieldOfView;
}

void AMGP_2526Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMGP_2526Character::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMGP_2526Character::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMGP_2526Character::Look);

		// Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMGP_2526Character::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMGP_2526Character::EndSprint);

		// Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMGP_2526Character::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMGP_2526Character::EndCrouch);

		//Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &AMGP_2526Character::OnZoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &AMGP_2526Character::EndZoom);
	}
	else
	{
		UE_LOG(LogMGP_2526, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

EDir AMGP_2526Character::CheckDirection()
{
	FVector v = GetVelocity();
	FRotator r = GetActorRotation();

	float MovementDirection = UKismetAnimationLibrary::CalculateDirection(v, r);
	float AbsMovDir = abs(MovementDirection);
	UE_LOG(LogTemp, Warning, TEXT("%f"), MovementDirection);

	if (UKismetMathLibrary::InRange_FloatFloat(AbsMovDir, 0, 45, true, true)) //0 is forwards, so this is within 30 degrees either way of that
	{
		return EDir::For;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(AbsMovDir, 45, 135, false, true))
	{
		return EDir::Sid;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(AbsMovDir, 135, 180, false, true))
	{
		return EDir::Bac;
	}
	else
	{
		return EDir::For; //If we -for some reason- fail to find a dir, just keep the player at default speed
	}
}

void AMGP_2526Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMGP_2526Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMGP_2526Character::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		FindMovementSpeed();

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMGP_2526Character::FindMovementSpeed() 
{
	// Implement a "current walk speed" that can be augmented (walk/run speed with a multiplier based on the direction). Update MaxWalkSpeed at the end of the function.
	EDir dir = CheckDirection();
	float currentWalkSpeed = Custom3DCMC->Walk_MaxWalkSpeed;

	if (dir == EDir::For) 
	{
			currentWalkSpeed *= 1;
	}
	else if (dir == EDir::Sid)
	{
		currentWalkSpeed *= 0.75;
	}
	else if (dir == EDir::Bac)
	{
		currentWalkSpeed *= 0.5;
	}

	//this->GetCharacterMovement()->MaxWalkSpeed = currentWalkSpeed;
}

void AMGP_2526Character::StartSprint() 
{
	if (Custom3DCMC) 
	{
		Custom3DCMC->bWantsToSprint = true;
	}
}

void AMGP_2526Character::EndSprint()
{
	if (Custom3DCMC)
	{
		Custom3DCMC->bWantsToSprint = false;
	}
}

void AMGP_2526Character::StartCrouch() 
{
	GetCharacterMovement()->bWantsToCrouch = true;
}
void AMGP_2526Character::EndCrouch() 
{
	GetCharacterMovement()->bWantsToCrouch = false;
}


void AMGP_2526Character::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMGP_2526Character::OnZoom() 
{
	bIsZoomed = true;
}
void AMGP_2526Character::EndZoom() 
{
	bIsZoomed = false;
}
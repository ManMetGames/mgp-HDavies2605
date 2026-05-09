// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPCharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DirEnum.h"

UMGPCharacterMovementComponent::UMGPCharacterMovementComponent()
{
}

void UMGPCharacterMovementComponent::BeginPlay() 
{
	Super::BeginPlay();
	Owner = GetOwner();
}

void UMGPCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	float speedModifier = FindMovementSpeedMod();

	//initial speed based on if walking/sprinting
	if (MovementMode == MOVE_Walking)
	{
		if (bWantsToSprint&&bCanSprint&&!bIsZoomed)
		{
			currentWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else
		{
			currentWalkSpeed = Walk_MaxWalkSpeed;
		}
	}

	MaxWalkSpeed = currentWalkSpeed * speedModifier;
		
}

float UMGPCharacterMovementComponent::FindMovementSpeedMod()
{
	if (bIsZoomed) { return 0.5f;}

	// Implement a "current walk speed" that can be augmented (walk/run speed with a multiplier based on the direction). Update MaxWalkSpeed at the end of the function.
	EDir dir = CheckDirection();
	switch (dir) 
	{
	case EDir::For:
		bCanSprint = true;
		return 1.0f;

	case EDir::Sid:
		bCanSprint = false;
		return 0.75f;

	case EDir::Bac:
		bCanSprint = false;
		return 0.5f;
	default:
		bCanSprint = false;
		return 1.0f;
	}
}

EDir UMGPCharacterMovementComponent::CheckDirection()
{
	//Check if velocity is above a certain amount, avoids jitter
	if (Velocity.SizeSquared() < 1.f)
	{
		return EDir::For;
	}

	FVector v = Owner->GetVelocity(); 
	FRotator r = Owner->GetActorRotation();

	float MovementDirection = UKismetAnimationLibrary::CalculateDirection(v, r);
	float AbsMovDir = abs(MovementDirection);
	UE_LOG(LogTemp, Warning, TEXT("%f"), MovementDirection);

	if (UKismetMathLibrary::InRange_FloatFloat(AbsMovDir, 0, 45, true, true)) //0 is forwards, so this is within 45 degrees either way of that
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
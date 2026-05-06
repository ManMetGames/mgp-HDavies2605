// Fill out your copyright notice in the Description page of Project Settings.


#include "Custom3DCMC.h"

UCustom3DCMC::UCustom3DCMC()
{

}

void UCustom3DCMC::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	UE_LOG(LogTemp, Warning, TEXT("bWantsToSprint: %d, MovementMode: %d"), bWantsToSprint, MovementMode);

	if (MovementMode == MOVE_Walking) 
	{
		if (bWantsToSprint) 
		{
			MaxWalkSpeed = Sprint_MaxWalkSpeed;
		}
		else 
		{
			MaxWalkSpeed = Walk_MaxWalkSpeed;
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MGPCharacterMovementComponent.generated.h"

/**
 *
 */

UCLASS()
class MGP_2526_API UMGPCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	//this CMC does NOT implement a lot of the expected safety features in the normal CMC and as such is not suitable for use in an online game

public:

	bool bWantsToSprint = false;

	UMGPCharacterMovementComponent();

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
};

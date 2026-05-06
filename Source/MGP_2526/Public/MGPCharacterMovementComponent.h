// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DirEnum.h"
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
	bool bWantsToParkour = false;

	UMGPCharacterMovementComponent();

	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly) bool canZoom;
	AActor* Owner;

protected:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	/**Find the movement speed modifier based on direction and determines if the player can sprint based on direction **/
	float FindMovementSpeedMod();

	/**Checks the direction the attached actor is moving and returns it as an EDir Enum (for, sid, bac) **/
	EDir CheckDirection();

	void ParkourFunc();

private:
	bool bCanSprint = false;
	float currentWalkSpeed = 0.0f;
	bool bCanParkour = true;
};

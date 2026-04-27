// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovementStateBaseState.generated.h"

/**
 * 
 * 
 */
class ACharacter;
UCLASS(Abstract, NotBlueprintable)
class MGP_2526_API UMovementStateBaseState : public UObject
{
	GENERATED_BODY()
protected:
	ACharacter* attachedCharacter;
public:
	void SetController(ACharacter* ControllerMain);
	virtual void EnterState();
	virtual void ExitState();
	virtual void UpdateState(float DeltaTime);
	virtual void HandleState();
};

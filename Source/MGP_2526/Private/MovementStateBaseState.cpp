// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementStateBaseState.h"

void UMovementStateBaseState::SetController(ACharacter* ControllerMain)
{
	attachedCharacter = ControllerMain;
}
void UMovementStateBaseState::EnterState() {}
void UMovementStateBaseState::ExitState() {}
void UMovementStateBaseState::UpdateState(float DeltaTime) {}
void UMovementStateBaseState::HandleState() {}

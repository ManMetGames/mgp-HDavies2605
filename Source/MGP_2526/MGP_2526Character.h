// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MGP_2526Character.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UENUM(BlueprintType)
enum class EDir : uint8
{
	For UMETA(DisplayName = "Forward"),
	Sid UMETA(DisplayName = "Sides"),
	Bac UMETA(DisplayName = "Back"),
};

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AMGP_2526Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

public:

	/** Constructor */
	AMGP_2526Character();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called whenever move is to find what speed to move at **/
	void FindMovementSpeed();

	bool bIsSprinting = false; //ist falsch

	bool bCanSprint = false;

	

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Checks the direction the player is heading while moving **/
	EDir CheckDirection();

	/** Called when sprint starts **/
	void StartSprint();

	/** Called when sprint ends **/
	void EndSprint();

	/** Called when Crouch begins **/
	UFUNCTION()
	void StartCrouch();

	UFUNCTION()
	void EndCrouch();

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float maxWalkSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	float maxRunSpeed;

	/**Called when Camera Zoom starts **/
	void OnZoom();

	/**Called when Camera Zoom ends **/
	void EndZoom();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};


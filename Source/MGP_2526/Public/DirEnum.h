// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DirEnum.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDir : uint8
{
	For UMETA(DisplayName = "Forward"),
	Sid UMETA(DisplayName = "Sides"),
	Bac UMETA(DisplayName = "Back"),
};

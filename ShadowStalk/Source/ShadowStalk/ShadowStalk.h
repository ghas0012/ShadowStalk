// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPickupType : uint8 {
	Undefined UMETA(DisplayName = "Undefined"),
	Key UMETA(DisplayName = "Key"),
	Item UMETA(DisplayName = "Item")
};

UENUM(BlueprintType)
enum class EEntityType : uint8 {
	Undefined UMETA(DisplayName = "Undefined"),
	Shade UMETA(DisplayName = "Shade"),
	MistWalker UMETA(DisplayName = "MistWalker")
};

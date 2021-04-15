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
enum class EItemType : uint8 {
	Undefined UMETA(DisplayName = "Undefined"),
	TestItem1 UMETA(DisplayName = "TestItem1"),
	TestItem2 UMETA(DisplayName = "TestItem2"),
};

UENUM(BlueprintType)
enum class EEntityType : uint8 {
	Undefined UMETA(DisplayName = "Undefined"),
	Shade UMETA(DisplayName = "Shade"),
	MistWalker UMETA(DisplayName = "MistWalker")
};

enum EInputLockFlags : uint8 {
	Interact   = 0x01,
	Crawl	   = 0x02,
	Sprint	   = 0x04,
	Blink	   = 0x08,
	Attack	   = 0x10,
	Jump	   = 0x20,
	MouseLook  = 0x40,
	Movement   = 0x80,
	Everything = 0xFF
};


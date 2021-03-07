// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STK_MatchGameState.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_MatchGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	void Register_MaxKeyCount(uint8 count);

	uint8 GetCurrentKeyCount();
	void Register_KeyPickedUp();
	void Register_KeyDropped(uint8 count);
	void Register_SelectedExitDoor(class ASTK_ExitDoor* ExitDoor);

	void OnAllKeysPickedUp();
	void OnKeysDropped();


protected:

	uint8 Max_Key_Count;
	uint8 Current_Key_Count;
	ASTK_ExitDoor* Selected_Exit_Door;

};

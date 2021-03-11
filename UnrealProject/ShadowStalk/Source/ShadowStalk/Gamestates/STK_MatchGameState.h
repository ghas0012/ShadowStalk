// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.
// - Added a list for entities, plus getters for both the monster and shades

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STK_MatchGameState.generated.h"

class ASTK_Entity;
class ASTK_EntityShade;

UCLASS()
class SHADOWSTALK_API ASTK_MatchGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	TArray <ASTK_Entity*> GetEntities();
	TArray <ASTK_EntityShade*> GetShades();
	class ASTK_EntityMonster* GetMonster();

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

	TArray <ASTK_Entity*> Entities;

	virtual void BeginPlay() override;

};

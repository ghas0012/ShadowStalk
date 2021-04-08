// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: 

	This class is intended to handle the game based on the rules set forward by MatchGameMode.
	It opens and closes the exit door, and provides an avenue for other scripts to access entities in the map with ease.

  ChangeLog:
  H 3/12/2021: Class init.
  H 3/12/2021: Added a list for entities, plus getters for both the monster and shades
  H 3/16/2021: Added a class description and summaries to relevant methods.
  C 4/07/2021: Added Updated some stuff to make the doors work on the Network. 
*/

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

	ASTK_MatchGameState();

	TArray <ASTK_Entity*> GetEntities();
	TArray <ASTK_EntityShade*> GetShades();
	class ASTK_EntityMonster* GetMonster();

	void Register_MaxKeyCount(uint8 count);

	uint8 GetCurrentKeyCount();

	void Register_KeyPickedUp();
	void Register_KeyDropped(uint8 count);
	void Register_SelectedExitDoor(class ASTK_ExitDoor* ExitDoor);

	void Register_NewEntity(APawn* entity);

	void OnAllKeysPickedUp();
	void OnKeysDropped();

protected:

	uint8 Max_Key_Count;
	uint8 Current_Key_Count;
	ASTK_ExitDoor* Selected_Exit_Door;

	TArray <ASTK_Entity*> Entities;

	virtual void BeginPlay() override;

};

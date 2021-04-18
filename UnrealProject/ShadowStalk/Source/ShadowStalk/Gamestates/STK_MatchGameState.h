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
  H 4/12/2021: Swapped to character. includes changes to lookat, moveto, execution, animation blueprint, attack, controllers, gamemode, gamestate.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "STK_MatchGameState.generated.h"

class ASTK_EntityCharacter;
class ASTK_EntityCharacterShade;

UCLASS()
class SHADOWSTALK_API ASTK_MatchGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	ASTK_MatchGameState();

	TArray <ASTK_EntityCharacter*> GetEntities();
	TArray <ASTK_EntityCharacterShade*> GetShades();
	class ASTK_EntityCharacterMonster* GetMonster();

	void Register_MaxKeyCount(uint8 count);

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentKeyCount() { return Current_Key_Count; };

	UFUNCTION(BlueprintCallable)
	uint8 GetMaxKeyCount() { return Max_Key_Count; };

	void Register_KeyPickedUp();
	void Register_KeyDropped(uint8 count);
	void Register_SelectedExitDoor(class ASTK_ExitDoor* ExitDoor);

	void Register_NewEntity(APawn* entity);

	void OnKeysDropped();

protected:

	uint8 Max_Key_Count;
	uint8 Current_Key_Count;
	ASTK_ExitDoor* Selected_Exit_Door;

	TArray <ASTK_EntityCharacter*> Entities;

	virtual void BeginPlay() override;

};

// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: 
  
	This class handles player spawning and posessing proper pawns.
	It's also in charge of setting up the level by selecting a preset number of spawn points to spawn keys from,
	and selecting a random exit door from a list of available doors. It then Informs the GameState of the results of the randomization.

  ChangeLog:
  H 3/12/2021: Init.
  H 3/15/2021: Added ability to spawn as the Monster. Added functionality to spawn only if the level has loaded.
  H 3/16/2021: Added a class description and summaries to relevant methods.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShadowStalk/Pickups/STK_PickupKey.h"
//#include "ShadowStalk/Pickups/STK_PickupItem.h"
#include "STK_MatchGameMode.generated.h"

class ASTK_PickupKey;
class ASTK_ExitDoor;
class ASTK_PickupSpawn;
class ASTK_Entity;

UCLASS()
class SHADOWSTALK_API ASTK_MatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logistics")
		bool PlayAsMonster;

	ASTK_MatchGameMode();

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	TArray<class APlayerController*> PlayerControllerList;

	uint8 PlayerCount = 0;
	
	bool bLevelHasLoaded = false;

protected:

	void SpawnPawnAndPosess(APlayerController* NewPlayer);

	FTimerHandle SpawnDelayHandle;

	void DelaySpawnUntilLevelLoaded();

	// UNCOMMENT STUFF TO HAVE ITEMS ADDED!

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logistics")
	TSubclassOf<ASTK_PickupKey> Pickup_Key_Template;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logistics")
	//	TSubclassOf<class ASTK_PickupKey> Pickup_Item_Template;

	ASTK_ExitDoor* SelectedExitDoor;
	TArray <ASTK_ExitDoor*> ExitDoorList;
	TArray <FVector> SpawnList_Key;
	//TArray <FVector> SpawnList_Item;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
	//uint8 Pickup_Item_Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
		uint8 Pickup_Key_Count;

	virtual void BeginPlay() override;


	TSubclassOf <APawn> pShadeBP;
	TSubclassOf <APawn> pMonsterBP;
	TSubclassOf <APlayerController> pShadeControllerBP;
	TSubclassOf <APlayerController> pMonsterControllerBP;

public:

	void RegisterPickupSpawnPoint(ASTK_PickupSpawn* PickupSpawn);
	void RegisterPotentialExitDoor(ASTK_ExitDoor* ExitDoor);

};

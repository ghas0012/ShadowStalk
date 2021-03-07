// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShadowStalk/Pickups/STK_PickupKey.h"
//#include "ShadowStalk/Pickups/STK_PickupItem.h"
#include "STK_MatchGameMode.generated.h"

class ASTK_PickupKey;
class ASTK_ExitDoor;
class ASTK_PickupSpawn;

UCLASS()
class SHADOWSTALK_API ASTK_MatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

protected:

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

public:

	void RegisterPickupSpawnPoint(ASTK_PickupSpawn* PickupSpawn);
	void RegisterPotentialExitDoor(ASTK_ExitDoor* ExitDoor);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShadowStalk/Pickups/STK_PickupKey.h"
//#include "ShadowStalk/Pickups/STK_PickupItem.h"
#include "STK_MatchGameMode.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_MatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

protected:

	// UNCOMMENT STUFF TO HAVE ITEMS ADDED!

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logistics")
		TSubclassOf<class ASTK_PickupKey> Pickup_Key_Template;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Logistics")
	//	TSubclassOf<class ASTK_PickupKey> Pickup_Item_Template;

	TArray <FVector> PickupSpawn_Key;
	//TArray <FVector> PickupSpawn_Item;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
	//uint8 Pickup_Item_Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
	uint8 Pickup_Key_Count;

	virtual void BeginPlay() override;

public:

	void RegisterPickupSpawnPoint(class ASTK_PickupSpawn* PickupSpawn);
};

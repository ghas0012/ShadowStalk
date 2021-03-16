// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_MatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/Pickups/STK_PickupSpawn.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"

void ASTK_MatchGameMode::RegisterPickupSpawnPoint(ASTK_PickupSpawn* PickupSpawn)
{
	switch (PickupSpawn->GetPickupType())
	{
		case EPickupType::Key:
			SpawnList_Key.Add(PickupSpawn->GetActorLocation());
			break;
		
		//case EPickupType::Item:
		//	PickupSpawn_Item.Add(PickupSpawn->GetActorLocation());
		//	break;
	}
}

void ASTK_MatchGameMode::RegisterPotentialExitDoor(ASTK_ExitDoor* ExitDoor)
{
	ExitDoorList.Add(ExitDoor);
}

void ASTK_MatchGameMode::BeginPlay()
{
	// What we do is we select randomly from our spawn locations and remove those as we go.
	// If we run out of spawn locations, refill the temp array and print a warning.

	Super::BeginPlay();

	if (SpawnList_Key.Num() == 0)
		return;

	TArray <FVector> TempArray;

	for (size_t i = 0; i < SpawnList_Key.Num(); i++)
	{
		TempArray.Add(SpawnList_Key[i]);
	}
	
	FActorSpawnParameters SpawnParams;

	for (size_t i = 0; i < Pickup_Key_Count; i++)
	{
		if (TempArray.Num() == 0)
		{
			for (size_t j = 0; j < SpawnList_Key.Num(); j++)
			{
				TempArray.Add(SpawnList_Key[j]);
			}
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("THERE ARE MORE KEYS THAN KEY SPAWN POINTS!"));
		}
		
		uint8 PickedIndex = FMath::RandRange(0, TempArray.Num()-1);

		FVector PickedLocation = TempArray[PickedIndex];
		TempArray.RemoveAt(PickedIndex);

		if (Pickup_Key_Template != nullptr)
		{
			GetWorld()->SpawnActor<AActor>(Pickup_Key_Template, PickedLocation, FRotator(0,0,0), SpawnParams);
		}
	}

	//for (size_t i = 0; i < Pickup_Item_Count; i++)
	//{
	//
	//}

	ASTK_MatchGameState* gamestate = GetGameState<ASTK_MatchGameState>();

	if (ExitDoorList.Num() > 0)
	{
		SelectedExitDoor = ExitDoorList[FMath::RandRange(0, ExitDoorList.Num() - 1)];
		gamestate->Register_SelectedExitDoor(SelectedExitDoor);
	}

	gamestate->Register_MaxKeyCount(Pickup_Key_Count);
}

void ASTK_MatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "STK_MatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/Pickups/STK_PickupSpawn.h"

void ASTK_MatchGameMode::RegisterPickupSpawnPoint(ASTK_PickupSpawn* PickupSpawn)
{
	switch (PickupSpawn->GetPickupType())
	{
		case EPickupType::Key:
			PickupSpawn_Key.Add(PickupSpawn->GetActorLocation());
			break;
		
		//case EPickupType::Item:
		//	PickupSpawn_Item.Add(PickupSpawn->GetActorLocation());
		//	break;
	}
}

void ASTK_MatchGameMode::BeginPlay()
{
	// What we do is we select randomly from our spawn locations and remove those as we go.
	// If we run out of spawn locations, refill the temp array and print a warning.

	if (PickupSpawn_Key.Num() == 0)
		return;

	TArray <FVector> TempArray;

	for (size_t i = 0; i < PickupSpawn_Key.Num(); i++)
	{
		TempArray.Add(PickupSpawn_Key[i]);
	}
	
	FActorSpawnParameters SpawnParams;

	for (size_t i = 0; i < Pickup_Key_Count; i++)
	{
		if (TempArray.Num() == 0)
		{
			for (size_t j = 0; j < PickupSpawn_Key.Num(); j++)
			{
				TempArray.Add(PickupSpawn_Key[j]);
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
}
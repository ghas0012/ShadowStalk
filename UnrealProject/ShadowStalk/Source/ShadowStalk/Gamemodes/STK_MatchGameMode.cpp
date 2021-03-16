// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_MatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/Pickups/STK_PickupSpawn.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "GameFramework/SpectatorPawn.h"

#include "ShadowStalk/Controllers/STK_EntityShadeController.h"
#include "ShadowStalk/Controllers/STK_EntityMonsterController.h"

#include "Engine/LevelStreaming.h"


/// <summary>
/// Grab the Monster and shade Blueprints and controllers on startup.
/// </summary>
ASTK_MatchGameMode::ASTK_MatchGameMode()
{
	// Get all our required bps and controllers
	static ConstructorHelpers::FClassFinder<APawn> MonsterPawnBP_Getter(TEXT("/Game/Blueprints/Entities/BP_EntityMonster"));
	static ConstructorHelpers::FClassFinder<APlayerController> MonsterControllerBP_Getter(TEXT("/Game/Blueprints/Misc/BP_MonsterController"));
	static ConstructorHelpers::FClassFinder<APawn> ShadePawnBP_Getter(TEXT("/Game/Blueprints/Entities/BP_EntityShade"));
	static ConstructorHelpers::FClassFinder<APlayerController> ShadeControllerBP_Getter(TEXT("/Game/Blueprints/Misc/BP_ShadeController"));

	if (MonsterPawnBP_Getter.Class != NULL)
	{
		pMonsterBP = MonsterPawnBP_Getter.Class;
	}

	if (MonsterControllerBP_Getter.Class != NULL)
	{
		pMonsterControllerBP = MonsterControllerBP_Getter.Class;
	}

	if (ShadePawnBP_Getter.Class != NULL)
	{
		pShadeBP = ShadePawnBP_Getter.Class;
	}

	if (ShadeControllerBP_Getter.Class != NULL)
	{
		pShadeControllerBP = ShadeControllerBP_Getter.Class;
	}

	bStartPlayersAsSpectators = true;
}


/// <summary>
/// Intercept players at login to assign controllers to them.
/// </summary>
APlayerController* ASTK_MatchGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	DefaultPawnClass = ASpectatorPawn::StaticClass();

	if (PlayAsMonster)
	{
		PlayerControllerClass = pMonsterControllerBP;
	}
	else
	{
		PlayerControllerClass = pShadeControllerBP;
	}

	return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
}


/// <summary>
/// Intercept players PostLogin, wait until level has loaded before spawning their proper pawn based on their controller.
/// </summary>
void ASTK_MatchGameMode::PostLogin(APlayerController* NewPlayer) {

	PlayerCount++;

	PlayerControllerList.Add(NewPlayer);
	NewPlayer->bBlockInput = true;

	Super::PostLogin(NewPlayer);

	if (!bLevelHasLoaded)
	{
		DelaySpawnUntilLevelLoaded();
	}
	else
	{
		SpawnPawnAndPosess(NewPlayer);
	}
}


/// <summary>
/// Check if the level has loaded every half a second. If it has, spawn every player contoller submitted.
/// </summary>
void ASTK_MatchGameMode::DelaySpawnUntilLevelLoaded()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Checking level loaded to spawn."));

	TArray<ULevelStreaming*> levels = GetWorld()->GetStreamingLevels();
	int LoadedCount = 0;

	if (levels.Num() != 0)
	{
		for (size_t i = 0; i < GetWorld()->GetStreamingLevels().Num(); i++)
		{
			if(levels[i]->GetCurrentState() == ULevelStreaming::ECurrentState::LoadedVisible)
			LoadedCount += 1;
		}
	}

	bLevelHasLoaded = LoadedCount == GetWorld()->GetStreamingLevels().Num();

	if (bLevelHasLoaded)
	{
		// Spawn all shade players.
		for (size_t i = 0; i < PlayerControllerList.Num(); i++)
		{
			SpawnPawnAndPosess(PlayerControllerList[i]);
		}

		GetWorldTimerManager().ClearTimer(SpawnDelayHandle);

		return;
	}

	GetWorldTimerManager().SetTimer(SpawnDelayHandle, this, &ASTK_MatchGameMode::DelaySpawnUntilLevelLoaded, 0.5f, false);
}


/// <summary>
/// Helper function to spawn and posess the correct pawn based on the player's assigned controller.
/// </summary>
void ASTK_MatchGameMode::SpawnPawnAndPosess(APlayerController* NewPlayer)
{
	if (ASTK_EntityMonsterController* monsterController = dynamic_cast<ASTK_EntityMonsterController*>(NewPlayer))
	{
		APawn* oldPawn = monsterController->GetPawnOrSpectator();

		monsterController->UnPossess();

		DefaultPawnClass = pMonsterBP;
		RestartPlayer(monsterController);

		if (oldPawn)
			oldPawn->Destroy();

		monsterController->bBlockInput = false;
	}
	else if (ASTK_EntityShadeController* shadeController = dynamic_cast<ASTK_EntityShadeController*>(NewPlayer))
	{
		APawn* oldPawn = shadeController->GetPawnOrSpectator();


		shadeController->UnPossess();

		DefaultPawnClass = pShadeBP;
		RestartPlayer(shadeController);

		if (oldPawn)
			oldPawn->Destroy();

		shadeController->bBlockInput = false;
	}
}


/// <summary>
/// Stores a pickup spawner's position.
/// </summary>
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


/// <summary>
/// Stores a potential exit door to potentially open when all keys are collected.
/// </summary>
void ASTK_MatchGameMode::RegisterPotentialExitDoor(ASTK_ExitDoor* ExitDoor)
{
	ExitDoorList.Add(ExitDoor);
}


/// <summary>
/// Spawns Pickup_Key_Count number of keys randomly on the provided spawn points.
/// Selects one door randomly from the registered doors provided.
/// Registers that door and the number of keys to pick with MatchGameState.
/// </summary>
void ASTK_MatchGameMode::BeginPlay()
{

	// What we do is we select randomly from our spawn locations and remove those as we go.
	// If we run out of spawn locations, refill the temp array and print a warning.

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

	// Spawn items here.
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
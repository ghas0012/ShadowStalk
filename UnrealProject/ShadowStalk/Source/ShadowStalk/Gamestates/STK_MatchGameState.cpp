// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_MatchGameState.h"
#include "ShadowStalk/GameModes/STK_MatchGameMode.h"
#include "ShadowStalk/ExitDoor/STK_ExitDoor.h"
#include "ShadowStalk/Entity/STK_Entity.h"
#include "ShadowStalk/Entity/STK_EntityShade.h"
#include "ShadowStalk/Entity/STK_EntityMonster.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/ShadowStalk.h"

/// <summary>
/// Informs the gamestate of a new key being picked up.
/// If enough keys are picked up, call OnAllKeysPickedUp().
/// </summary>
void ASTK_MatchGameState::Register_KeyPickedUp()
{
    // safety so it doesn't go above 255
    int finalCount = ++Current_Key_Count;
    FMath::Clamp(finalCount, 0, 255);

    Current_Key_Count = finalCount;
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, FString::Printf(TEXT("Gamestate: Key picked up. %d / %d"), Current_Key_Count, Max_Key_Count));

    if (Current_Key_Count >= Max_Key_Count)
    {
        OnAllKeysPickedUp();
        //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Gamestate: Attempting to open door."));
    }
}


/// <summary>
/// Informs the gamestate of a key being dropped.
/// Calls OnKeysDropped().
/// </summary>
void ASTK_MatchGameState::Register_KeyDropped(uint8 count)
{
    // safety so it doesn't go below 0
    int finalCount = Current_Key_Count - count;
    FMath::Clamp(finalCount, 0, 255);

    Current_Key_Count = finalCount;
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Gamestate: Key dropped. %d / %d"), Current_Key_Count, Max_Key_Count));

    OnKeysDropped();
}


/// <summary>
/// A gameplay scripting helper function to get all the Shade instances in the map.
/// </summary>
TArray<class ASTK_EntityShade*> ASTK_MatchGameState::GetShades()
{
    TArray<class ASTK_EntityShade*> to_return;

    for (size_t i = 0; i < Entities.Num(); i++)
    {
        if (Entities[i]->GetEntityType() == EEntityType::Shade)
        to_return.Add( Cast<ASTK_EntityShade>(Entities[i]) );
    }

    return to_return;
}


/// <summary>
/// A gameplay scripting helper function to get all the Entities in the map.
/// </summary>
TArray<ASTK_Entity*> ASTK_MatchGameState::GetEntities()
{
    TArray<AActor*> found;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTK_Entity::StaticClass(), found);

    Entities.Empty();

    for (size_t i = 0; i < found.Num(); i++)
    {
        Entities.Add(Cast<ASTK_Entity>(found[i]));
    }

    return Entities;
}


/// <summary>
/// A gameplay scripting helper function to get the first Monster entity if finds in the map.
/// </summary>
ASTK_EntityMonster* ASTK_MatchGameState::GetMonster()
{
    for (size_t i = 0; i < Entities.Num(); i++)
    {
        if (Entities[i]->GetEntityType() == EEntityType::MistWalker)
        return Cast<ASTK_EntityMonster>(Entities[i]);
    }

    return nullptr;
}


/// <summary>
/// A gameplay scripting helper function to get the first Monster entity if finds in the map.
/// </summary>
void ASTK_MatchGameState::Register_MaxKeyCount(uint8 count)
{
    Max_Key_Count = count;
}


/// <summary>
/// Store the specific door to open.
/// </summary>
void ASTK_MatchGameState::Register_SelectedExitDoor(ASTK_ExitDoor* ExitDoor)
{
    Selected_Exit_Door = ExitDoor;
}


/// <summary>
/// Opens the selected door if all keys are picked up.
/// </summary>
void ASTK_MatchGameState::OnAllKeysPickedUp()
{
    if (Selected_Exit_Door)
        Selected_Exit_Door->DoorOpen();
}


/// <summary>
/// Closes the selected door if a key was dropped.
/// </summary>
void ASTK_MatchGameState::OnKeysDropped()
{
    if (Selected_Exit_Door)
        Selected_Exit_Door->DoorClose();

    //GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Gamestate: Attempting to close door"), Current_Key_Count));
}


/// <summary>
/// Get all the entities in the map on beginplay. MUST BE UPDATED.
/// </summary>
void ASTK_MatchGameState::BeginPlay()
{
    // TODO: MAKE THE GAMEMODE UPDATE THE ACTIVE ENTITIES IN THE MAP ON THE GAMESTATE. THIS WILL ALLOW NEW PLAYERS TO BE COUNTED.
    Super::BeginPlay();
    GetEntities();
}

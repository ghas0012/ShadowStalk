// Fill out your copyright notice in the Description page of Project Settings.

#include "STK_MatchGameState.h"
#include "ShadowStalk/GameModes/STK_MatchGameMode.h"
#include "ShadowStalk/ExitDoor/STK_ExitDoor.h"

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
        GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Gamestate: Attempting to open door."));
    }
}

void ASTK_MatchGameState::Register_KeyDropped(uint8 count)
{
    // safety so it doesn't go below 0
    int finalCount = Current_Key_Count - count;
    FMath::Clamp(finalCount, 0, 255);

    Current_Key_Count = finalCount;
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Gamestate: Key dropped. %d / %d"), Current_Key_Count, Max_Key_Count));

    OnKeysDropped();
}

void ASTK_MatchGameState::Register_MaxKeyCount(uint8 count)
{
    Max_Key_Count = count;
}

void ASTK_MatchGameState::Register_SelectedExitDoor(ASTK_ExitDoor* ExitDoor)
{
    Selected_Exit_Door = ExitDoor;
}

void ASTK_MatchGameState::OnAllKeysPickedUp()
{
    if (Selected_Exit_Door)
        Selected_Exit_Door->DoorOpen();
}

void ASTK_MatchGameState::OnKeysDropped()
{
    if (Selected_Exit_Door)
        Selected_Exit_Door->DoorClose();

    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Gamestate: Attempting to close door"), Current_Key_Count));
}


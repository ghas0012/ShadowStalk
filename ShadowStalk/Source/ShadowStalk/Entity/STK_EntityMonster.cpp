// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young

// Changelog:
// - Class init.

#include "STK_EntityMonster.h"
#include "Components/CapsuleComponent.h"

ASTK_EntityMonster::ASTK_EntityMonster()
{
    //Default Monster Stats 
    m_JumpStrength = 10000.0f;
    m_Acceleration = 1750.0f;
    m_MaxWalkSpeed = 400.0f;
    m_MaxSprintSpeed = 800.0f;
    m_AirControl = 0.0f;
    m_CapsuleHalfHeight = 100.0f;
    m_CapsuleRadius = 50.0f;
}


void ASTK_EntityMonster::Interact()
{

}

E_MonsterState ASTK_EntityMonster::GetMonsterState()
{
    return CurrentState;
}

void ASTK_EntityMonster::SetMonsterState(E_MonsterState state)
{
    CurrentState = state;
}

void ASTK_EntityMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTK_EntityMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

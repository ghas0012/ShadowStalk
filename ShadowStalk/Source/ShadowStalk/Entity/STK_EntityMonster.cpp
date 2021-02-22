// Fill out your copyright notice in the Description page of Project Settings.


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


void ASTK_EntityMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTK_EntityMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

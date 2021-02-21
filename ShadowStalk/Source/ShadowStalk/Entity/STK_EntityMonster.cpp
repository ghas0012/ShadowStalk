// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_EntityMonster.h"
#include "Components/CapsuleComponent.h"

ASTK_EntityMonster::ASTK_EntityMonster()
{
    m_JumpStrength = 100.0f;
    m_Acceleration = 50.0f;
    m_MaxWalkSpeed = 200.0f;
    m_AirControl = 10.0f;
}

void ASTK_EntityMonster::Forward(float value)
{
    Super::Forward(value);
}

void ASTK_EntityMonster::Strafe(float value)
{
    Super::Strafe(value);
}

void ASTK_EntityMonster::Jump()
{
    Super::Jump();
}

void ASTK_EntityMonster::Interact()
{

}

void ASTK_EntityMonster::MouseLook_Vertical(float value)
{
    Super::MouseLook_Vertical(value);
}

void ASTK_EntityMonster::MouseLook_Horizontal(float value)
{
    Super::MouseLook_Horizontal(value);
}

void ASTK_EntityMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTK_EntityMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

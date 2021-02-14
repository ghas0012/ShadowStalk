// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityMonster.h"
#include "Components/CapsuleComponent.h"

AEntityMonster::AEntityMonster()
{

}

void AEntityMonster::Forward(float value)
{
    Super::Forward(value);
}

void AEntityMonster::Strafe(float value)
{
    Super::Strafe(value);
}

void AEntityMonster::Jump()
{
    Super::Jump();
}

void AEntityMonster::Interact()
{
    
}

void AEntityMonster::MouseLook_Vertical(float value)
{
    Super::MouseLook_Vertical(value);
}

void AEntityMonster::MouseLook_Horizontal(float value)
{
    Super::MouseLook_Horizontal(value);
}

void AEntityMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEntityMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

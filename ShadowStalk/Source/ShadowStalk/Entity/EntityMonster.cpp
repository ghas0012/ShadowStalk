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
    //TODO 
}

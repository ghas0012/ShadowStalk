// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityMonsterController.h"

#include "../Entity/STK_EntityMonster.h"


/// <summary>
/// Grab the posessing pawn to send input to.
/// </summary>
void ASTK_EntityMonsterController::OnPossess(APawn* aPawn)
{
    //Temp Code, depending on how Networking stuff works with Possession.

    Super::OnPossess(aPawn);

    m_MonsterEntity = Cast<ASTK_EntityMonster>(aPawn);

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Monster Name: " + m_MonsterEntity->GetName());
}


void ASTK_EntityMonsterController::OnUnPossess()
{
    //Temp Code, depending on how Networking stuff works with Possession.
    APawn* possessedPawn = Cast<APawn>(GetOwner());
    Super::OnUnPossess();
}

/// <summary>
/// Binds relevant input from the engine into the entity class.
/// </summary>
void ASTK_EntityMonsterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAxis("Forward", this, &ASTK_EntityMonsterController::Forward);
        InputComponent->BindAxis("Strafe", this, &ASTK_EntityMonsterController::Strafe);

        InputComponent->BindAction("Jump", IE_Pressed, this, &ASTK_EntityMonsterController::Jump);
        InputComponent->BindAction("Attack", IE_Pressed, this, &ASTK_EntityMonsterController::Attack);
        InputComponent->BindAction("Interact", IE_Pressed, this, &ASTK_EntityMonsterController::Interact);

        InputComponent->BindAction("Sprint", IE_Pressed, this, &ASTK_EntityMonsterController::SetSprint);
        InputComponent->BindAction("Sprint", IE_Released, this, &ASTK_EntityMonsterController::StopSprint);

        InputComponent->BindAxis("MouseLook_Vertical", this, &ASTK_EntityMonsterController::MouseLook_Vertical);
        InputComponent->BindAxis("MouseLook_Horizontal", this, &ASTK_EntityMonsterController::MouseLook_Horizontal);

        InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASTK_EntityMonsterController::PauseMenu);
    }
}


void ASTK_EntityMonsterController::Forward(float value)
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Forward(value);
    }
}


void ASTK_EntityMonsterController::Strafe(float value)
{
    if(m_MonsterEntity)
    {
        m_MonsterEntity->Strafe(value);
    }
}


void ASTK_EntityMonsterController::Jump()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Jump();
    }
}


void ASTK_EntityMonsterController::Interact()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Interact();
    }
}


void ASTK_EntityMonsterController::Attack()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Attack();
    }
}


void ASTK_EntityMonsterController::SetSprint()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Sprint(true);
    }
}


void ASTK_EntityMonsterController::StopSprint()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Sprint(false);
    }
}


void ASTK_EntityMonsterController::MouseLook_Vertical(float value)
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->MouseLook_Vertical(value);
    }
}


void ASTK_EntityMonsterController::MouseLook_Horizontal(float value)
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->MouseLook_Horizontal(value);
    }
}


void ASTK_EntityMonsterController::PauseMenu()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->PauseMenu();
    }
}
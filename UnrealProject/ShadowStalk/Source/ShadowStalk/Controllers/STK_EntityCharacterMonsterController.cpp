// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityCharacterMonsterController.h"

#include "../Entity/STK_EntityCharacterMonster.h"
#include "Net/UnrealNetwork.h"

/// <summary>
/// Grab the posessing pawn to send input to.
/// </summary>
void ASTK_EntityCharacterMonsterController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);
}

/// <summary>
/// Sets up input.
/// </summary>
void ASTK_EntityCharacterMonsterController::AcknowledgePossession(APawn* aPawn)
{
    Super::AcknowledgePossession(aPawn);

    m_MonsterEntityCharacter = Cast<ASTK_EntityCharacterMonster>(aPawn);
}

void ASTK_EntityCharacterMonsterController::OnUnPossess()
{
    //Temp Code, depending on how Networking stuff works with Possession.
    APawn* possessedPawn = Cast<APawn>(GetOwner());
    Super::OnUnPossess();
}

/// <summary>
/// Binds relevant input from the engine into the entity class.
/// </summary>
void ASTK_EntityCharacterMonsterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAxis("Forward", this, &ASTK_EntityCharacterMonsterController::Forward);
        InputComponent->BindAxis("Strafe", this, &ASTK_EntityCharacterMonsterController::Strafe);

        InputComponent->BindAction("Jump", IE_Pressed, this, &ASTK_EntityCharacterMonsterController::Jump);
        InputComponent->BindAction("Attack", IE_Pressed, this, &ASTK_EntityCharacterMonsterController::Attack);
        InputComponent->BindAction("Interact", IE_Pressed, this, &ASTK_EntityCharacterMonsterController::Interact);
        InputComponent->BindAction("DropTrap", IE_Pressed, this, &ASTK_EntityCharacterMonsterController::DropTrap);

        InputComponent->BindAction("Sprint", IE_Pressed, this, &ASTK_EntityCharacterMonsterController::SetSprint);
        InputComponent->BindAction("Sprint", IE_Released, this, &ASTK_EntityCharacterMonsterController::StopSprint);

        InputComponent->BindAxis("MouseLook_Vertical", this, &ASTK_EntityCharacterMonsterController::MouseLook_Vertical);
        InputComponent->BindAxis("MouseLook_Horizontal", this, &ASTK_EntityCharacterMonsterController::MouseLook_Horizontal);

        InputComponent->BindAxis("MouseLook_Vertical", this, &ASTK_EntityCharacterMonsterController::MouseLook_Vertical);
        InputComponent->BindAxis("MouseLook_Horizontal", this, &ASTK_EntityCharacterMonsterController::MouseLook_Horizontal);

        //InputComponent->BindAxis("Turn", this, &ASTK_EntityCharacterShadeController::MouseLook_Horizontal);
        //InputComponent->BindAxis("LookUp", this, &ASTK_EntityCharacterShadeController::MouseLook_Vertical);
        InputComponent->BindAxis("TurnRate", m_MonsterEntityCharacter, &ASTK_EntityCharacterMonster::TurnAtRate);
        InputComponent->BindAxis("LookUpRate", m_MonsterEntityCharacter, &ASTK_EntityCharacterMonster::LookUpAtRate);

        InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASTK_EntityCharacterMonsterController::PauseMenu);
    }
}


void ASTK_EntityCharacterMonsterController::Forward(float value)
{
    if(m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Forward(value);
    }
}


void ASTK_EntityCharacterMonsterController::Strafe(float value)
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Strafe(value);
    }
}


void ASTK_EntityCharacterMonsterController::Jump()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Jump();
    }
}


void ASTK_EntityCharacterMonsterController::Interact()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Interact();
    }
}


void ASTK_EntityCharacterMonsterController::Attack()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Attack();
    }
}


void ASTK_EntityCharacterMonsterController::SetSprint()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Sprint(true);
    }
}


void ASTK_EntityCharacterMonsterController::StopSprint()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->Sprint(false);
    }
}


void ASTK_EntityCharacterMonsterController::MouseLook_Vertical(float value)
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->MouseLook_Vertical(value);
    }
}


void ASTK_EntityCharacterMonsterController::MouseLook_Horizontal(float value)
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->MouseLook_Horizontal(value);
    }
}

void ASTK_EntityCharacterMonsterController::PauseMenu()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->PauseMenu();
    }
}

void ASTK_EntityCharacterMonsterController::DropTrap()
{
    if (m_MonsterEntityCharacter != nullptr)
    {
        m_MonsterEntityCharacter->SpawnTrap();
    }
}

void ASTK_EntityCharacterMonsterController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASTK_EntityCharacterMonsterController, m_MonsterEntityCharacter);
}


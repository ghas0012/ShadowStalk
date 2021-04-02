// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityShadeController.h"

#include "../Entity/STK_EntityShade.h"


/// <summary>
/// Grab the posessing pawn to send input to.
/// </summary>
void ASTK_EntityShadeController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);

    m_ShadeEntity = Cast<ASTK_EntityShade>(aPawn);
}


void ASTK_EntityShadeController::OnUnPossess()
{
    APawn* possessedPawn = Cast<APawn>(GetOwner());

    Super::OnUnPossess();
}

/// <summary>
/// Binds relevant input from the engine into the entity class.
/// </summary>
void ASTK_EntityShadeController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent != nullptr)
    {
        InputComponent->BindAxis("Forward", this, &ASTK_EntityShadeController::Forward);
        InputComponent->BindAxis("Strafe", this, &ASTK_EntityShadeController::Strafe);

        InputComponent->BindAction("Jump", IE_Pressed, this, &ASTK_EntityShadeController::Jump);

        InputComponent->BindAction("Sprint", IE_Pressed, this, &ASTK_EntityShadeController::SetSprint);
        InputComponent->BindAction("Sprint", IE_Released, this, &ASTK_EntityShadeController::StopSprint);

        InputComponent->BindAction("Crawl", IE_Pressed, this, &ASTK_EntityShadeController::SetCrawl);
        InputComponent->BindAction("Crawl", IE_Released, this, &ASTK_EntityShadeController::UnsetCrawl);

        InputComponent->BindAxis("MouseLook_Vertical", this, &ASTK_EntityShadeController::MouseLook_Vertical);
        InputComponent->BindAxis("MouseLook_Horizontal", this, &ASTK_EntityShadeController::MouseLook_Horizontal);

        InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASTK_EntityShadeController::PauseMenu);
    }
}


void ASTK_EntityShadeController::Forward(float value)
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Forward(value);
    }
}


void ASTK_EntityShadeController::Strafe(float value)
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Strafe(value);
    }
}


void ASTK_EntityShadeController::Jump()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Jump();
    }
}


void ASTK_EntityShadeController::SetSprint()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Sprint(true);
    }
}


void ASTK_EntityShadeController::StopSprint()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Sprint(false);
    }
}


void ASTK_EntityShadeController::SetCrawl()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Crawl(true);
    }
}


void ASTK_EntityShadeController::UnsetCrawl()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Crawl(false);
    }
}


void ASTK_EntityShadeController::Interact()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->Interact();
    }
}


void ASTK_EntityShadeController::MouseLook_Vertical(float value)
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->MouseLook_Vertical(value);
    }
}


void ASTK_EntityShadeController::MouseLook_Horizontal(float value)
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->MouseLook_Horizontal(value);
    }
}


void ASTK_EntityShadeController::PauseMenu()
{
    if (m_ShadeEntity)
    {
        m_ShadeEntity->PauseMenu();
    }
}

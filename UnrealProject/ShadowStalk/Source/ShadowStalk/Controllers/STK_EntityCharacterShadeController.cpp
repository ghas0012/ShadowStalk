// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityCharacterShadeController.h"

#include "../Entity/STK_EntityCharacterShade.h"
#include "Net/UnrealNetwork.h"

#include "../Inventory/STK_InventoryComponent.h"


/// <summary>
/// Grab the posessing pawn to send input to.
/// </summary>
void ASTK_EntityCharacterShadeController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);

}


void ASTK_EntityCharacterShadeController::OnUnPossess()
{
    APawn* possessedPawn = Cast<APawn>(GetOwner());

    Super::OnUnPossess();
}

void ASTK_EntityCharacterShadeController::AcknowledgePossession(APawn* aPawn)
{
    Super::AcknowledgePossession(aPawn);

    m_ShadeEntityCharacter = Cast<ASTK_EntityCharacterShade>(aPawn);

    SetupInputComponent();
}

/// <summary>
/// Binds relevant input from the engine into the entity class.
/// </summary>
void ASTK_EntityCharacterShadeController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent != nullptr && m_ShadeEntityCharacter)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Bound Shade input"));

        InputComponent->BindAxis("Forward", this, &ASTK_EntityCharacterShadeController::Forward);
        InputComponent->BindAxis("Strafe", this, &ASTK_EntityCharacterShadeController::Strafe);

        InputComponent->BindAction("Jump", IE_Pressed, this, &ASTK_EntityCharacterShadeController::Jump);
        InputComponent->BindAction("Sprint", IE_Pressed, this, &ASTK_EntityCharacterShadeController::SetSprint);

        InputComponent->BindAction("Crawl", IE_Pressed, this, &ASTK_EntityCharacterShadeController::SetCrawl);
        InputComponent->BindAction("Crawl", IE_Released, this, &ASTK_EntityCharacterShadeController::UnsetCrawl);

        InputComponent->BindAxis("MouseLook_Vertical", this, &ASTK_EntityCharacterShadeController::MouseLook_Vertical);
        InputComponent->BindAxis("MouseLook_Horizontal", this, &ASTK_EntityCharacterShadeController::MouseLook_Horizontal);

        InputComponent->BindAxis("TurnRate", m_ShadeEntityCharacter, &ASTK_EntityCharacterShade::TurnAtRate);
        InputComponent->BindAxis("LookUpRate", m_ShadeEntityCharacter, &ASTK_EntityCharacterShade::LookUpAtRate);

        InputComponent->BindAction("CloseEyes", IE_Pressed, this, &ASTK_EntityCharacterShadeController::CloseEyes);
        InputComponent->BindAction("CloseEyes", IE_Released, this, &ASTK_EntityCharacterShadeController::OpenEyes);
        InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASTK_EntityCharacterShadeController::PauseMenu);
        InputComponent->BindAction("NextItem", IE_Pressed, m_ShadeEntityCharacter, &ASTK_EntityCharacterShade::NextItem);
        InputComponent->BindAction("PreviousItem", IE_Pressed, m_ShadeEntityCharacter, &ASTK_EntityCharacterShade::PrevItem);
    }
}


void ASTK_EntityCharacterShadeController::Forward(float value)
{
    if (m_ShadeEntityCharacter)
    if (m_ShadeEntityCharacter != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Forward, %f"), value));
        m_ShadeEntityCharacter->Forward(value);
    }
}


void ASTK_EntityCharacterShadeController::Strafe(float value)
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Strafe, %f"), value));
        m_ShadeEntityCharacter->Strafe(value);
    }
}


void ASTK_EntityCharacterShadeController::Jump()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->Jump();
    }
}


void ASTK_EntityCharacterShadeController::SetSprint()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        if (isSprint)
        {
            isSprint = false;
        }
        else
        {
            isSprint = true;
        }

        m_ShadeEntityCharacter->Sprint(isSprint);
    }
}




//TODO - Make Sprint toggle optional.

//void ASTK_EntityCharacterShadeController::StopSprint()
//{
//  if (m_ShadeEntityCharacter)
//  {
//  	m_ShadeEntityCharacter->Sprint(false);
//  }
//}


void ASTK_EntityCharacterShadeController::SetCrawl()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->Crawl(true);
    }
}


void ASTK_EntityCharacterShadeController::UnsetCrawl()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->Crawl(false);
    }
}


void ASTK_EntityCharacterShadeController::Interact()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->Interact();
    }
}


void ASTK_EntityCharacterShadeController::MouseLook_Vertical(float value)
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->MouseLook_Vertical(value);
    }
}


void ASTK_EntityCharacterShadeController::MouseLook_Horizontal(float value)
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->MouseLook_Horizontal(value);
    }
}

void ASTK_EntityCharacterShadeController::CloseEyes()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->Blink(true);
    }
}

void ASTK_EntityCharacterShadeController::OpenEyes()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->Blink(false);
    }
}

void ASTK_EntityCharacterShadeController::PauseMenu()
{
    if (m_ShadeEntityCharacter != nullptr)
    {
        m_ShadeEntityCharacter->PauseMenu();
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Escape hit, ack | " + m_ShadeEntityCharacter->GetName());
    }
}

void ASTK_EntityCharacterShadeController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTK_EntityCharacterShadeController, m_ShadeEntityCharacter);
}

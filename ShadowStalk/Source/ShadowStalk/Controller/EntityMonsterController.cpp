// Fill out your copyright notice in the Description page of Project Settings.


#include "EntityMonsterController.h"
#include "../Entity/EntityMonster.h"

void AEntityMonsterController::OnPossess(APawn* aPawn)
{
    //Temp Code, depending on how Networking stuff works with Possession.

    Super::OnPossess(aPawn);

    m_MonsterEntity = Cast<AEntityMonster>(aPawn);

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Monster Name: " + m_MonsterEntity->GetName());

}

void AEntityMonsterController::OnUnPossess()
{
    //Temp Code, depending on how Networking stuff works with Possession.
    APawn* possessedPawn = Cast<APawn>(GetOwner());
    Super::OnUnPossess();
}

void AEntityMonsterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (InputComponent)
    {
        InputComponent->BindAxis("Forward", this, &AEntityMonsterController::Forward);
        InputComponent->BindAxis("Strafe", this, &AEntityMonsterController::Strafe);
        InputComponent->BindAction("Jump", IE_Pressed, this, &AEntityMonsterController::Jump);
        InputComponent->BindAction("Interact", IE_Pressed, this, &AEntityMonsterController::Interact);
    }
}

void AEntityMonsterController::Forward(float value)
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Forward(value);
    }
}

void AEntityMonsterController::Strafe(float value)
{
    if(m_MonsterEntity)
    {
        m_MonsterEntity->Strafe(value);
    }
}

void AEntityMonsterController::Jump()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Jump();
    }
}

void AEntityMonsterController::Interact()
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->Interact();
    }
}

void AEntityMonsterController::MouseLook_Vertical(float value)
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->MouseLook_Vertical(value);
    }
}

void AEntityMonsterController::MouseLook_Horizontal(float value)
{
    if (m_MonsterEntity)
    {
        m_MonsterEntity->MouseLook_Horizontal(value);
    }
}

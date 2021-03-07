// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young, Hamidreza Ghasemi

// Changelog:
// - Class init.
// - Added basic attack logic.

#include "STK_EntityMonster.h"
#include "Components/CapsuleComponent.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityShade.h"

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

void ASTK_EntityMonster::BeginPlay()
{
    Super::BeginPlay();
    gamestate = GetWorld()->GetGameState<ASTK_MatchGameState>();
}

void ASTK_EntityMonster::Attack()
{
    TArray<ASTK_EntityShade*> Shades = gamestate->GetShades();

    for (size_t i = 0; i < Shades.Num(); i++)
    {
        if (Shades[i]->GetShadeState() == EShadeState::Default)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("AttackDistance: %f"), FVector::Distance(Shades[i]->GetActorLocation(), GetActorLocation() + FVector::ForwardVector + FVector::ForwardVector * 200)));

            if (FVector::Distance(Shades[i]->GetActorLocation(), GetActorLocation() + FVector::ForwardVector + FVector::ForwardVector * 200) < 500)
            {
                Shades[i]->GetHurt(1);

                if (Shades[i]->GetRootComponent()->IsSimulatingPhysics())
                    Shades[i]->m_PlayerCapsule->AddImpulse((Shades[i]->GetActorLocation() - GetActorLocation()).GetSafeNormal() * 50000);
                
            }
        }
    }
}


void ASTK_EntityMonster::Interact()
{

}

EMonsterState ASTK_EntityMonster::GetMonsterState()
{
    return CurrentState;
}

void ASTK_EntityMonster::SetMonsterState(EMonsterState state)
{
    CurrentState = state;
}

EEntityType ASTK_EntityMonster::GetEntityType()
{
    return EEntityType::MistWalker;
}

void ASTK_EntityMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTK_EntityMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

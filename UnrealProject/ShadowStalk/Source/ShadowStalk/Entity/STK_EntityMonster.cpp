// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityMonster.h"
#include "Components/CapsuleComponent.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityShade.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"

ASTK_EntityMonster::ASTK_EntityMonster()
{
    //Default Monster Stats 
    m_JumpStrength = 10000.0f;
    m_Acceleration = 1750.0f;
    m_WalkSpeed = 400.0f;
    m_SprintSpeed = 800.0f;
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
    if (InputLockFlags & EInputLockFlags::Attack)
        return;

    TArray<ASTK_EntityShade*> Shades = gamestate->GetShades();

    if (Shades.Num() > 0)
    {
        for (size_t i = 0; i < Shades.Num(); i++)
        {
            EShadeState state = Shades[i]->GetShadeState();
            if (state == EShadeState::Default || state == EShadeState::Hurt)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, FString::Printf(TEXT("AttackDistance: %f"), FVector::Distance(Shades[i]->GetActorLocation(), GetActorLocation() + FVector::ForwardVector + FVector::ForwardVector * 200)));

                if (FVector::Distance(Shades[i]->GetActorLocation(), GetActorLocation() + m_CameraComp->GetForwardVector() * AttackRange) < ExecutionPositioningDistance)
                {
                    Shades[i]->ApplyDamage(1, (Shades[i]->GetActorLocation() - GetActorLocation()).GetSafeNormal() * AttackKnockbackStrength);
                }
            }
        }
    }

}


void ASTK_EntityMonster::Interact()
{
    if (InputLockFlags & EInputLockFlags::Interact)
        return;

    UWorld* World = GetWorld();
    if (World != nullptr)
    {
        FHitResult hit;
        FVector RayStart = m_CameraComp->GetComponentLocation();
        FVector RayEnd = RayStart + m_CameraComp->GetForwardVector() * GrabRange;
        ECollisionChannel trace = ECC_Pawn;
        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(this);

        //DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Red, false, 5.f, ECC_Pawn, 2.f);
        //FCollisionResponseParams responseParams;

        if (World->LineTraceSingleByChannel(hit, RayStart, RayEnd, trace, queryParams))
        {
            if (hit.bBlockingHit)
            {
                ASTK_EntityShade* TargetShade = Cast<ASTK_EntityShade>(hit.GetActor());

                if (TargetShade)
                {
                    if(TargetShade->GetShadeState() == EShadeState::Downed)
                    ExecuteShade(TargetShade);
                }
            }
        }
    }
}

void ASTK_EntityMonster::ExecuteShade(ASTK_EntityShade* TargetShade)
{ 
    if (TargetShade == nullptr)
        return;

    TargetShade->StartExecution(this);
    SetMonsterState(EMonsterState::Executing);
    
    LockCameraLookat(TargetShade->m_CameraComp->GetComponentLocation());
    GetWorldTimerManager().SetTimer(ExecutionTimerHandle, this, &ASTK_EntityMonster::OnExcecuteEnd, ExecutionTimeLength, false);

}
  
void ASTK_EntityMonster::OnExcecuteEnd()
{
    SetInputLock(EInputLockFlags::Everything, false);
    SetMonsterState(EMonsterState::Default);
    UnlockCameraLookat();
}

EMonsterState ASTK_EntityMonster::GetMonsterState()
{
    return CurrentState;
}

void ASTK_EntityMonster::SetMonsterState(EMonsterState state)
{

    switch (state)
    {
    case EMonsterState::Default:
        break;

    case EMonsterState::Executing:
        SetInputLock(EInputLockFlags::Everything, true);
        break;

    case EMonsterState::Stunned:
        break;

    default:
        break;
    }

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

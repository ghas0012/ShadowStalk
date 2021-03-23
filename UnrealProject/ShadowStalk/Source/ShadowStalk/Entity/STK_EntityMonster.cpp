// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityMonster.h"
#include "Components/CapsuleComponent.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityShade.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

//Sounds
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

ASTK_EntityMonster::ASTK_EntityMonster()
{
    //Default Monster Stats 
    m_MovementData.m_JumpStrength = 10000.0f;
    m_MovementData.m_Acceleration = 1750.0f;
    m_MovementData.m_WalkSpeed = 400.0f;
    m_MovementData.m_SprintSpeed = 800.0f;
    m_MovementData.m_AirControl = 0.0f;
    m_MovementData.m_CapsuleHalfHeight = 100.0f;
    m_MovementData.m_CapsuleRadius = 50.0f;
}

void ASTK_EntityMonster::BeginPlay()
{
    Super::BeginPlay();
    gamestate = GetWorld()->GetGameState<ASTK_MatchGameState>();
}


void ASTK_EntityMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


/// <summary>
/// Basic function to allow the monster to attack shades. Will be updated to have a cooldown, and be dependent on the monster's animation.
/// </summary>
void ASTK_EntityMonster::Attack()
{
    if (InputLockFlags & EInputLockFlags::Attack)
        return;

    //Play the sound
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), MonsterAttackScream, GetActorLocation());

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


/// <summary>
/// Allows the monster to interact with the world by firing a Line Trace. Currently, it's only used to execute downed shades.
/// </summary>
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
        ECollisionChannel trace = ECC_GameTraceChannel2;
        FCollisionQueryParams queryParams;
        queryParams.AddIgnoredActor(this);

        //DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Red, false, 5.f, ECC_Pawn, 2.f);

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


/// <summary>
/// Initiate execution of downed shades. It locks both the shade and monster's input, forces them to look at each other, and it sets their state for the animation blueprint to use.
/// </summary>
void ASTK_EntityMonster::ExecuteShade(ASTK_EntityShade* TargetShade)
{ 
    if (TargetShade == nullptr)
        return;

    TargetShade->StartExecution(this);

    SetInputLock(EInputLockFlags::Everything, true);
    SetMonsterState(EMonsterState::Executing);
    
    LockCameraLookat(TargetShade->m_CameraComp->GetComponentLocation());
    GetWorldTimerManager().SetTimer(ExecutionTimerHandle, this, &ASTK_EntityMonster::OnExcecuteEnd, ExecutionTimeLength, false);

}
  

/// <summary>
/// Unlocks the monster's input and resets their state after an execution.
/// </summary>
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


/// <summary>
/// Sets the monster's state. Also allows for custom functionality to apply when a certain state change occurs.
/// </summary>
void ASTK_EntityMonster::SetMonsterState(EMonsterState state)
{

    switch (state)
    {
    case EMonsterState::Default:
        break;

    case EMonsterState::Executing:
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


void ASTK_EntityMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

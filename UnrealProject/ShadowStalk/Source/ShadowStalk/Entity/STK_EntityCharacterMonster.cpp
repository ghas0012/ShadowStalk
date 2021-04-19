// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityCharacterMonster.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityCharacterShade.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameElements/STK_TrapBase.h"

#include "ShadowStalk/Controllers/STK_EntityCharacterMonsterController.h"

#include "GameFramework/CharacterMovementComponent.h"

//Sounds
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

ASTK_EntityCharacterMonster::ASTK_EntityCharacterMonster()
{
    //Default Monster Stats 
    m_MovementData.m_JumpStrength = 10000.0f;
    m_MovementData.m_Acceleration = 1750.0f;
    m_MovementData.m_WalkSpeed = 400.0f;
    m_MovementData.m_SprintSpeed = 800.0f;
    m_MovementData.m_AirControl = 1.0f;
    m_MovementData.m_CapsuleHalfHeight = 100.0f;
    m_MovementData.m_CapsuleRadius = 50.0f;

    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = false;

    numOfTraps = 5;

    Tags.Add("Monster");

    SetReplicates(true);
}

void ASTK_EntityCharacterMonster::BeginPlay()
{
    Super::BeginPlay();
    gamestate = GetWorld()->GetGameState<ASTK_MatchGameState>();
}

void ASTK_EntityCharacterMonster::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/// <summary>
/// Drop traps on server
/// </summary>
void ASTK_EntityCharacterMonster::SpawnTrap_Implementation()
{
    if (numOfTraps > 0)
    {
        FActorSpawnParameters SpawnParms;
        SpawnParms.Owner = this;

        FTransform trapLoc;
        trapLoc.SetLocation(GetActorLocation());

         ASTK_TrapBase* Trap = GetWorld()->SpawnActor<ASTK_TrapBase>(BearTrap, trapLoc, SpawnParms);

         Trap->SetInstigator(this);
         Trap->SpawnTrap();

         numOfTraps--;
    }
}

/// <summary>
/// Basic function to allow the monster to attack shades. Will be updated to have a cooldown, and be dependent on the monster's animation.
/// </summary>
void ASTK_EntityCharacterMonster::Attack()
{
    Server_Attack();
}

void ASTK_EntityCharacterMonster::Server_Attack_Implementation()
{
    if (InputLockFlags & EInputLockFlags::Attack)
        return;

    //Play the sound
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), MonsterAttackSound, GetActorLocation());

    TArray<ASTK_EntityCharacterShade*> Shades = gamestate->GetShades();

    if (Shades.Num() > 0)
    {
        for (size_t i = 0; i < Shades.Num(); i++)
        {
            ECharacterShadeState state = Shades[i]->GetShadeState();
            if (state == ECharacterShadeState::Default || state == ECharacterShadeState::Hurt)
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
void ASTK_EntityCharacterMonster::Interact()
{
    Server_Interact();
}

void ASTK_EntityCharacterMonster::Server_Interact_Implementation()
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
                ASTK_EntityCharacterShade* TargetShade = Cast<ASTK_EntityCharacterShade>(hit.GetActor());

                if (TargetShade)
                {
                    if (TargetShade->GetShadeState() == ECharacterShadeState::Downed)
                        ExecuteShade(TargetShade);
                }
            }
        }
    }
}

bool ASTK_EntityCharacterMonster::Server_Interact_Validate()
{
    return true;
}

/// <summary>
/// Initiate execution of downed shades. It locks both the shade and monster's input, forces them to look at each other, and it sets their state for the animation blueprint to use.
/// </summary>
void ASTK_EntityCharacterMonster::ExecuteShade(ASTK_EntityCharacterShade* TargetShade)
{ 
    Server_ExecuteShade(TargetShade);
}

void ASTK_EntityCharacterMonster::Server_ExecuteShade_Implementation(ASTK_EntityCharacterShade* TargetShade)
{
    if (TargetShade == nullptr)
        return;

    TargetShade->StartExecution(this);

    SetInputLock(EInputLockFlags::Everything, true);
    SetMonsterState(ECharacterMonsterState::Executing);

    LockCameraLookat(TargetShade->m_CameraComp);
    GetWorldTimerManager().SetTimer(ExecutionTimerHandle, this, &ASTK_EntityCharacterMonster::OnExcecuteEnd, ExecutionTimeLength, false);
}
  

/// <summary>
/// Unlocks the monster's input and resets their state after an execution.
/// </summary>
void ASTK_EntityCharacterMonster::OnExcecuteEnd()
{
    Server_OnExcecuteEnd();
}

void ASTK_EntityCharacterMonster::Server_OnExcecuteEnd_Implementation()
{
    SetInputLock(EInputLockFlags::Everything, false);
    SetMonsterState(ECharacterMonsterState::Default);
    UnlockCameraLookat();
}

ECharacterMonsterState ASTK_EntityCharacterMonster::GetMonsterState()
{
    return CurrentState;
}

/// <summary>
/// Sets the monster's state. Also allows for custom functionality to apply when a certain state change occurs.
/// </summary>
void ASTK_EntityCharacterMonster::SetMonsterState(ECharacterMonsterState state)
{

    switch (state)
    {
    case ECharacterMonsterState::Default:
        break;

    case ECharacterMonsterState::Executing:
        break;

    case ECharacterMonsterState::Stunned:

        break;

    default:
        break;
    }

    CurrentState = state;

}


EEntityType ASTK_EntityCharacterMonster::GetEntityType()
{
    return EEntityType::MistWalker;
}


void ASTK_EntityCharacterMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}


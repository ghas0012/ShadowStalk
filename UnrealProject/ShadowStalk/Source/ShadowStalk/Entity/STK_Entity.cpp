// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_Entity.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../MovementComponent/STK_EntityMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "DrawDebugHelpers.h"
#include "ShadowStalk/GameInstance/STK_GameInstance.h"
#include "ShadowStalk/UI/STK_UserWidget.h"


// Sets default values
ASTK_Entity::ASTK_Entity()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>("Player Capsule");
    m_PlayerCapsule->SetCapsuleRadius(m_MovementData.m_CapsuleRadius);
    m_PlayerCapsule->SetCapsuleHalfHeight(m_MovementData.m_CapsuleHalfHeight);
    m_PlayerCapsule->SetEnableGravity(true);
    m_PlayerCapsule->SetSimulatePhysics(true);
    m_PlayerCapsule->GetBodyInstance()->bLockYRotation;
    m_PlayerCapsule->GetBodyInstance()->bLockXRotation;
    m_PlayerCapsule->SetCollisionProfileName("Pawn");
    m_PlayerCapsule->SetLinearDamping(0.5f);
    m_PlayerCapsule->SetAngularDamping(1.0f);
    m_PlayerCapsule->BodyInstance.bLockXRotation = true;
    m_PlayerCapsule->BodyInstance.bLockYRotation = true;
    SetRootComponent(m_PlayerCapsule);

    m_CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Comp");
    m_CameraComp->SetProjectionMode(ECameraProjectionMode::Perspective);
    m_CameraComp->SetupAttachment(m_PlayerCapsule);

    m_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh Comp");
    m_MeshComp->bHiddenInGame = false;
    m_MeshComp->SetRelativeRotation(FRotator(0, 180, 0));
    m_MeshComp->SetupAttachment(m_PlayerCapsule);

    m_MovementComp = CreateDefaultSubobject<USTK_EntityMovementComponent>("Movement Component");
    m_MovementComp->CapsuleComp = m_PlayerCapsule;
    m_MovementComp->UpdatedComponent = RootComponent;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void ASTK_Entity::BeginPlay()
{
    m_MovementComp->WalkSpeed = m_MovementData.m_WalkSpeed;
    m_MovementComp->AirControl = m_MovementData.m_AirControl;
    m_MovementComp->SprintSpeed = m_MovementData.m_SprintSpeed;
    m_MovementComp->CurrentSpeed = m_MovementData.m_WalkSpeed;
    m_MovementComp->Acceleration = m_MovementData.m_Acceleration;
    m_MovementComp->FrictionLerp = m_MovementData.m_FrictionLerp;
    m_MovementComp->CapsuleStandingHalfHeight = m_MovementData.m_CapsuleHalfHeight;
    m_MovementComp->CapsuleCrawlHalfHeight = FMath::Max(m_MovementData.m_CrawlCapsuleHalfHeight,
    m_MovementData.m_CapsuleRadius);
    m_MovementComp->CrawlSpeed = m_MovementData.m_CrawlSpeed;
    m_MovementComp->MeshComp = m_MeshComp;

    MouseLookVector.Y = m_CameraComp->GetRelativeRotation().Pitch;
    MouseLookVector.X = m_PlayerCapsule->GetRelativeRotation().Yaw;

    Super::BeginPlay();
}


/// <summary>
/// The Tick function handles camera rotation, position, and footstep sounds.
/// </summary>
void ASTK_Entity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    HandleCamera(DeltaTime);
    HandleFootstepSounds(DeltaTime);
    HandlePosition(DeltaTime);
}


/// <summary>
/// Record forward acceleration value. Lockable.
/// </summary>
void ASTK_Entity::Forward(float value)
{
    if (InputLockFlags & EInputLockFlags::Movement || bPositionOverride)
    {
        RightAccelerationVector = FVector(0);
        return;
    }

    ForwardAccelerationVector = GetActorForwardVector() * value;
    //AddMovementInput(GetActorForwardVector(), value);
}


/// <summary>
/// Record side acceleration value. Lockable.
/// </summary>
void ASTK_Entity::Strafe(float value)
{
    if (InputLockFlags & EInputLockFlags::Movement || bPositionOverride)
    {
        RightAccelerationVector = FVector(0);
        return;
    }

    RightAccelerationVector = GetActorRightVector() * value;
}


/// <summary>
/// Forces the camera to look at a location.
/// </summary>
void ASTK_Entity::LockCameraLookat(FVector Location)
{
    bCameraOverride = true;
    CameraOverrideTarget = Location;
}


/// <summary>
/// Releases the camera if it's locked.
/// </summary>
void ASTK_Entity::UnlockCameraLookat()
{
    bCameraOverride = false;
}


/// <summary>
/// Overrides the entity's position, moving it to a specified point.
/// </summary>
void ASTK_Entity::ForceMoveToPoint(FVector target)
{
    bPositionOverride = true;
    PositionOverrideTarget = target;
    PositionOverrideOrigin = m_PlayerCapsule->GetRelativeLocation();
}


void ASTK_Entity::Interact()
{
    // LEAVE EMPTY, Will get overridden by child classes.
}


/// <summary>
/// Passes the jump request to the movement component. Lockable.
/// </summary>
void ASTK_Entity::Jump()
{
    if (InputLockFlags & EInputLockFlags::Jump)
        return;

    if (m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
    {
        if (m_MovementComp->bIsGrounded)
        {
            m_MovementComp->Jump(m_MovementData.m_JumpStrength);
            m_MovementComp->bIsGrounded = false;
        }
    }
}


/// <summary>
/// Passes the sprint requests to the movement component. Lockable and Toggleable.
/// </summary>
void ASTK_Entity::Sprint(bool IsSprint)
{
    if (InputLockFlags & EInputLockFlags::Sprint)
        return;

    if (IsSprint == true)
    {
        m_MovementComp->Sprint();
    }
    else
    {
        m_MovementComp->Walk();
    }
}


/// <summary>
/// Passes the crawl requests to the movement component. Lockable and Toggleable.
/// </summary>
void ASTK_Entity::Crawl(bool IsCrawl)
{
    if (InputLockFlags & EInputLockFlags::Crawl)
        return;

    if (IsCrawl == true)
    {
        m_MovementComp->Crawl();
    }
    if (IsCrawl == false)
    {
        m_MovementComp->Walk();
    }
}


/// <summary>
/// Records vertical mouse input for HandleCamera(). Lockable.
/// </summary>
void ASTK_Entity::MouseLook_Vertical(float value)
{
    if (InputLockFlags & EInputLockFlags::MouseLook)
        return;

    MouseLookVector.Y += m_MovementData.m_MouseLook_Y * value;
    //GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouselook Y : %f "), MouseLookVector.Y));
}


/// <summary>
/// Records horizontal mouse input for HandleCamera(). Lockable. 
/// </summary>
void ASTK_Entity::MouseLook_Horizontal(float value)
{
    if (InputLockFlags & EInputLockFlags::MouseLook)
        return;

    MouseLookVector.X += m_MovementData.m_MouseLook_X * value;
    //GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouselook X : %f "), MouseLookVector.X));
}


/// <summary>
/// Creates and sets up the Pause Menu Widget in the game's viewport.
/// </summary>
void ASTK_Entity::SetupPauseMenu()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());
    if (GameInstance == nullptr) return;

    GameInstance->SetupPauseMenuWidget();
}


bool ASTK_Entity::GetIsGrounded()
{
    return bIsGrounded;
}


EEntityType ASTK_Entity::GetEntityType()
{
    return EEntityType::Undefined;
}


/// <summary>
/// This function applies camera and collider rotation based on MouseLookVector.
/// If the camera's rotation is being overridden, it rotates the camera to face CamOverrideTarget.
/// </summary>
void ASTK_Entity::HandleCamera(float DeltaTime)
{
    if (!bCameraOverride)
    {

        m_PlayerCapsule->SetRelativeRotation(FRotator(0, MouseLookVector.X, 0));

        if (abs(MouseLookVector.Y) < m_MovementData.m_MouseLook_VerticalLookLimitAngle)
        {
            m_CameraComp->SetRelativeRotation(FRotator(MouseLookVector.Y, 0, 0));
        }
        else
        {
            MouseLookVector.Y = -m_MovementData.m_MouseLook_VerticalLookLimitAngle * (signbit(MouseLookVector.Y) * 2 - 1);
        }

    }
    else
    {

        FVector TargetPos = CameraOverrideTarget;
        FRotator Rot = (TargetPos - m_CameraComp->GetComponentLocation()).GetSafeNormal().Rotation();
        FRotator calcRot = FMath::RInterpTo(FRotator(MouseLookVector.Y, MouseLookVector.X, 0), Rot, DeltaTime, CameraOverrideSpeed);

        MouseLookVector.X = calcRot.Yaw;
        MouseLookVector.Y = calcRot.Pitch;

        m_PlayerCapsule->SetRelativeRotation(FRotator(0, MouseLookVector.X, 0));
        m_CameraComp->SetRelativeRotation(FRotator(MouseLookVector.Y, 0, 0));

    }
}


/// <summary>
/// This function passes the input acceleration to the movement component.
/// If the collider's position is being overridden, it moves the collider to PositionOverrideTarget.
/// </summary>
void ASTK_Entity::HandlePosition(float DeltaTime)
{
    if (!bPositionOverride)
    {
        FVector CombinedAccleration = ForwardAccelerationVector + RightAccelerationVector;
        if (!CombinedAccleration.IsNearlyZero() && m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
        {
            m_MovementComp->AddInputVector(CombinedAccleration);
        }
    }
    else
    {
        PositionOverridePercent += DeltaTime * PositionOverrideSpeed;

        if (PositionOverridePercent >= 1)
        {
            bPositionOverride = false;
            PositionOverridePercent = 0;
            RootComponent->SetRelativeLocation(PositionOverrideTarget);
            return;
        }

        RootComponent->SetRelativeLocation(
            FMath::Lerp(PositionOverrideOrigin, PositionOverrideTarget, PositionOverridePercent)
        );
    }
}


/// <summary>
/// This function handles footstep sounds. Will be updated to sync with the entity's animation.
/// </summary>
void ASTK_Entity::HandleFootstepSounds(float DeltaTime)
{
    FootstepTimer += DeltaTime * FootstepFrequency * m_MovementComp->GetForwardVelocity();
    if (m_MovementComp->GetIsGrounded() && FootstepTimer >= 1 && !AudioComponent->IsPlaying())
    {
        FootstepTimer = 0;

        //AudioComponent->SetSound(FootstepsSound);
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepsSound, GetActorLocation());
    }

}


/// <summary>
/// This function allows for input locking using flags. Look at EInputLockFlags in Shadowstalk.h for more info.
/// </summary>
void ASTK_Entity::SetInputLock(uint8 flag, bool lock)
{
    if (flag & EInputLockFlags::Movement)
    {
        m_MovementComp->StopMovementImmediately();
        ForwardAccelerationVector = FVector(0);
        RightAccelerationVector = FVector(0);
    }

    lock ? InputLockFlags |= flag : InputLockFlags &= ~flag;
}


/// <summary>
/// This function allows for input locking using flags. Look at EInputLockFlags in Shadowstalk.h for more info.
/// </summary>
void ASTK_Entity::SetInputLock(EInputLockFlags flag, bool lock)
{
    SetInputLock(static_cast<uint8>(flag), lock);
}


// Called to bind functionality to input
void ASTK_Entity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
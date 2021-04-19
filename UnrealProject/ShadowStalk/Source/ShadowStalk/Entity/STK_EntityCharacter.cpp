// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.


#include "STK_EntityCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"

#include "ShadowStalk/GameInstance/STK_GameInstance.h"

#include "ShadowStalk/UI/STK_UserWidget.h"
#include "ShadowStalk/UI/STK_UWPauseMenu.h"

#include "../Inventory/STK_InventoryComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Net/UnrealNetwork.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASTK_EntityCharacter::ASTK_EntityCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //CapsuleComponent->SetCapsuleRadius(m_MovementData.m_CapsuleRadius);
    //CapsuleComponent->SetCapsuleHalfHeight(m_MovementData.m_CapsuleHalfHeight);
    //CapsuleComponent->SetEnableGravity(true);
    //CapsuleComponent->SetSimulatePhysics(true);
    //CapsuleComponent->GetBodyInstance()->bLockYRotation;
    //CapsuleComponent->GetBodyInstance()->bLockXRotation;
    //CapsuleComponent->SetCollisionProfileName("Pawn");
    //CapsuleComponent->SetLinearDamping(0.5f);
    //CapsuleComponent->SetAngularDamping(1.0f);
    //CapsuleComponent->BodyInstance.bLockXRotation = true;
    //CapsuleComponent->BodyInstance.bLockYRotation = true;
    //SetRootComponent(m_PlayerCapsule);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    m_CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Comp");
    m_CameraComp->SetProjectionMode(ECameraProjectionMode::Perspective);
    m_CameraComp->bUsePawnControlRotation = true;
    m_CameraComp->SetupAttachment(GetCapsuleComponent());
    // m_CameraComp->bUsePawnControlRotation = true;

    GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...

    m_TPMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("TP Mesh Comp");
    m_TPMeshComp->bHiddenInGame = false;
    m_TPMeshComp->SetRelativeRotation(FRotator(0, 180, 0));
    m_TPMeshComp->SetOwnerNoSee(true);
    m_TPMeshComp->SetupAttachment(GetCapsuleComponent());

    GetMesh()->bHiddenInGame = false;
    GetMesh()->SetRelativeRotation(FRotator(0, 180, 0));
    GetMesh()->SetOnlyOwnerSee(true);
    GetMesh()->SetupAttachment(GetCapsuleComponent());

    if (GetLocalRole() == ROLE_AutonomousProxy)
    {
        GetMesh()->CastShadow = 0;
    }

    //m_MovementComp = CreateDefaultSubobject<USTK_EntityMovementComponent>("Movement Component");
    //m_MovementComp->CapsuleComp = m_PlayerCapsule;
    //m_MovementComp->UpdatedComponent = RootComponent;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    AudioComponent->SetupAttachment(RootComponent);

    InventoryComponent = CreateDefaultSubobject<USTK_InventoryComponent>("Inventory");

    ////bReplicates = true;
    ////SetReplicatingMovement(true);
    SetReplicates(true);

    //Search for Pause Menu Widget Blueprint
    {
        ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/UI/WBP_PauseMenu"));
        if (!ensure(PauseMenuBPClass.Class != nullptr)) return;

        PauseMenuClass = PauseMenuBPClass.Class;
    }
}

/// <summary>
/// Controller support for rotation
/// </summary>
void ASTK_EntityCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * m_MovementData.m_MouseLook_X * GetWorld()->GetDeltaSeconds());
}

/// <summary>
/// Controller support for Lookup
/// </summary>
void ASTK_EntityCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * m_MovementData.m_MouseLook_Y * GetWorld()->GetDeltaSeconds());
}

/// <summary>
/// Sets up character related variables. done in beginplay so shade and monster can override the numbers in their constructor.
/// </summary>
void ASTK_EntityCharacter::BeginPlay()
{
    GetCharacterMovement()->JumpZVelocity = m_MovementData.m_JumpStrength;
    GetCharacterMovement()->MaxAcceleration = m_MovementData.m_Acceleration;
    GetCharacterMovement()->MaxWalkSpeed = m_MovementData.m_WalkSpeed;
    GetCharacterMovement()->MaxWalkSpeedCrouched = m_MovementData.m_CrawlSpeed;
    GetCharacterMovement()->CrouchedHalfHeight = m_MovementData.m_CrawlCapsuleHalfHeight;
    GetCharacterMovement()->AirControl = m_MovementData.m_AirControl;

    GetCapsuleComponent()->SetCapsuleRadius(m_MovementData.m_CapsuleRadius);
    GetCapsuleComponent()->SetCapsuleHalfHeight(m_MovementData.m_CapsuleHalfHeight);

    Super::BeginPlay();
}

void ASTK_EntityCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    HandleFootstepSounds(DeltaTime);
    HandlePositionOverride(DeltaTime);
    HandleCamera(DeltaTime);
}

/// <summary>
/// Helper function to allow for position overriding. otherwise normal movement applies.
/// </summary>
void ASTK_EntityCharacter::HandlePositionOverride(float DeltaTime)
{
    if (bPositionOverride)
    {
        if (PositionOverridePercent >= 1.0f)
        {
            SetActorLocation(PositionOverrideTarget);
            PositionOverridePercent = 1;
            bPositionOverride = false;
            return;
        }

        SetActorLocation(FMath::Lerp(PositionOverrideOrigin, PositionOverrideTarget, PositionOverridePercent));
        PositionOverridePercent += DeltaTime * PositionOverrideSpeed;
    }
}

/// <summary>
/// Allows us to override what the player's looking at.
/// used mainly to rotate the player on execution.
/// </summary>
void ASTK_EntityCharacter::HandleCamera(float DeltaTime)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentControllerFacing = GetControlRotation();
    }

    if (bCameraOverride)
    {
        FRotator targetRotation = (CameraOverrideTarget->GetComponentLocation() - m_CameraComp->GetComponentLocation()).Rotation();

        if (CameraOverridePercent >= 1.0f)
        {
            GetController()->SetControlRotation(targetRotation);
            CameraOverridePercent = 1;
            bCameraOverride = false;
            return;
        }

        GetController()->SetControlRotation(FMath::Lerp(CameraOverrideOrigin, targetRotation, CameraOverridePercent));
        CameraOverridePercent += DeltaTime * CameraOverrideSpeed;
    }
}

/// <summary>
/// Plays footstep sounds based on velocity.
/// </summary>
void ASTK_EntityCharacter::HandleFootstepSounds(float DeltaTime)
{
    if (!IsLocallyControlled())
    {
        FootstepTimer += DeltaTime * FootstepFrequency * GetCapsuleComponent()->GetComponentVelocity().Size();
        if (GetMovementComponent()->IsMovingOnGround() && FootstepTimer >= 1 && PlayFootstep1 == true && !AudioComponent->IsPlaying())
        {
            FootstepTimer = 0;

            //AudioComponent->SetSound(FootstepsSound);
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepsSound, GetActorLocation(), 1.f, 1.f, 0.f, FootstepAttenuation);

            PlayFootstep1 = false;
            PlayFootstep2 = true;
        }

        if (GetMovementComponent()->IsMovingOnGround() && FootstepTimer >= 1 && PlayFootstep2 == true && !AudioComponent->IsPlaying())
        {
            FootstepTimer = 0;

            //AudioComponent->SetSound(FootstepsSound);
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepsSound2, GetActorLocation(), 1.f, 1.f, 0.f, FootstepAttenuation);

            PlayFootstep2 = false;
            PlayFootstep1 = true;
        }
    }
    else
    {
        FootstepTimer += DeltaTime * FootstepFrequency * GetCapsuleComponent()->GetComponentVelocity().Size();
        if (GetMovementComponent()->IsMovingOnGround() && FootstepTimer >= 1 && PlayFootstep1 == true && !AudioComponent->IsPlaying())
        {
            FootstepTimer = 0;

            //AudioComponent->SetSound(FootstepsSound);
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepsSound, GetActorLocation());

            PlayFootstep1 = false;
            PlayFootstep2 = true;
        }

        if (GetMovementComponent()->IsMovingOnGround() && FootstepTimer >= 1 && PlayFootstep2 == true && !AudioComponent->IsPlaying())
        {
            FootstepTimer = 0;

            //AudioComponent->SetSound(FootstepsSound);
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepsSound2, GetActorLocation());

            PlayFootstep2 = false;
            PlayFootstep1 = true;
        }
    }
}

/// <summary>
/// This function allows for input locking using flags. Look at EInputLockFlags in Shadowstalk.h for more info.
/// </summary>
void ASTK_EntityCharacter::SetInputLock(EInputLockFlags flag, bool lock)
{
    SetInputLock(static_cast<uint8>(flag), lock);
}

/// <summary>
/// This function allows for input locking using flags. Look at EInputLockFlags in Shadowstalk.h for more info.
/// </summary>
void ASTK_EntityCharacter::SetInputLock(uint8 flag, bool lock)
{
    Server_SetInputLock(flag, lock);
}

/// <summary>
/// Locks input. See above for explanation.
/// </summary>
void ASTK_EntityCharacter::Server_SetInputLock_Implementation(uint8 flag, bool lock)
{
    if (flag & EInputLockFlags::Movement)
    {
        GetCharacterMovement()->StopMovementImmediately();
    }

    lock ? InputLockFlags |= flag : InputLockFlags &= ~flag;
}

/// <summary>
/// Passes movement to the character controller
/// </summary>
void ASTK_EntityCharacter::Forward(float value)
{
    if (InputLockFlags & EInputLockFlags::Movement || bPositionOverride)
        return;

    ACharacter::AddMovementInput(GetActorForwardVector(), value);
}

/// <summary>
/// Passes movement to the character controller
/// </summary>
void ASTK_EntityCharacter::Strafe(float value)
{
    if (InputLockFlags & EInputLockFlags::Movement || bPositionOverride)
        return;

    ACharacter::AddMovementInput(GetActorRightVector(), value);
    //GetCharacterMovement()->AddInputVector(GetActorRightVector() * value);
}

/// <summary>
/// Virtual function. no base behavior.
/// </summary>
void ASTK_EntityCharacter::Interact()
{
}

/// <summary>
/// Passes jump requests to the character
/// </summary>
void ASTK_EntityCharacter::Jump()
{
    if (InputLockFlags & EInputLockFlags::Jump)
        return;

    ACharacter::Jump();
}

/// <summary>
/// Sets new speed based on whether or not we're sprinting. networked the speed value.
/// </summary>
void ASTK_EntityCharacter::Sprint(bool IsSprint)
{
    if (InputLockFlags & EInputLockFlags::Sprint)
        return;

    if (ACharacter::bIsCrouched)
    {
        Crawl(false);
    }

    GetCharacterMovement()->MaxWalkSpeed = IsSprint ? m_MovementData.m_SprintSpeed : m_MovementData.m_WalkSpeed;
    Server_Sprint(IsSprint);
}

void ASTK_EntityCharacter::Server_Sprint_Implementation(bool IsSprint)
{
    GetCharacterMovement()->MaxWalkSpeed = IsSprint ? m_MovementData.m_SprintSpeed : m_MovementData.m_WalkSpeed;
}

/// <summary>
/// Passes crawl requests to the character
/// </summary>
void ASTK_EntityCharacter::Crawl(bool IsCrawl)
{
    if (InputLockFlags & EInputLockFlags::Crawl)
        return;

    IsCrawl ? ACharacter::Crouch(true) : ACharacter::UnCrouch(true);
}

/// <summary>
/// Handles vertical mouselook
/// </summary>
void ASTK_EntityCharacter::MouseLook_Vertical(float value)
{
    if (InputLockFlags & EInputLockFlags::MouseLook || bCameraOverride)
        return;

    APawn::AddControllerPitchInput(-value * m_MovementData.m_MouseLook_X);
}

/// <summary>
/// Handles horizontal mouselook
/// </summary>
void ASTK_EntityCharacter::MouseLook_Horizontal(float value)
{
    if (InputLockFlags & EInputLockFlags::MouseLook || bCameraOverride)
        return;

    APawn::AddControllerYawInput(value * m_MovementData.m_MouseLook_X);
}

/// <summary>
/// Creates and sets up the Pause Menu Widget in the game's viewport.
/// </summary>
void ASTK_EntityCharacter::PauseMenu()
{
    if (PauseMenuClass)
    {

        // Create the pause menu widget
        UWPauseMenu = CreateWidget<USTK_UWPauseMenu>(GetWorld(), PauseMenuClass);
        UWPauseMenu->Setup();


        //TODO: User Interface [Fix Toggle Menu to avoid widget overlap (widget switcher)]
        /*if (UWPauseMenu)
        {
            UWPauseMenu->RemoveFromParent();
            //Change Input Method
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            PlayerController->SetInputMode(FInputModeGameOnly());
            PlayerController->bShowMouseCursor = false;
            UWPauseMenu = nullptr;
        }
        else
        {
            UWPauseMenu = CreateWidget<USTK_UWPauseMenu>(GetWorld(), PauseMenuClass);
            UWPauseMenu->AddToViewport();
            //Change Input Method
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            PlayerController->SetInputMode(FInputModeGameAndUI());
            PlayerController->bShowMouseCursor = true;
        }*/
    }
}

/// <summary>
/// Cycles to next inventory item
/// </summary>
void ASTK_EntityCharacter::NextItem()
{
    InventoryComponent->NextInventoryItem();
}

/// <summary>
/// Cycles to previous inventory item
/// </summary>
void ASTK_EntityCharacter::PrevItem()
{
    InventoryComponent->PreviousInventoryItem();
}

void ASTK_EntityCharacter::UseItem()
{
    // TODO
}

/// <summary>
/// Forces the client to rotate and face a requested scene comp (usually other camera components)
/// </summary>
void ASTK_EntityCharacter::LockCameraLookat(USceneComponent* SceneComp)
{
    Client_LockCameraLookat(SceneComp);
}

/// <summary>
/// Forces the client to rotate and face a requested scene comp (usually other camera components)
/// </summary>
void ASTK_EntityCharacter::Client_LockCameraLookat_Implementation(USceneComponent* SceneComp)
{
    bCameraOverride = true;
    CameraOverrideTarget = SceneComp;
    CameraOverrideOrigin = GetController()->GetControlRotation();
    CameraOverridePercent = 0;
}

/// <summary>
/// Releases the client's camera rotation
/// </summary>
void ASTK_EntityCharacter::UnlockCameraLookat()
{
    bCameraOverride = false;
}

/// <summary>
/// Forces the client to lerp to a requested fvector
/// </summary>
void ASTK_EntityCharacter::ForceMoveToPoint(FVector target)
{
    bPositionOverride = true;
    PositionOverrideTarget = target;
    PositionOverridePercent = 0;
    PositionOverrideOrigin = GetActorLocation();
}

// Called to bind functionality to input
void ASTK_EntityCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASTK_EntityCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTK_EntityCharacter, InputLockFlags);
    DOREPLIFETIME(ASTK_EntityCharacter, CurrentControllerFacing);
}

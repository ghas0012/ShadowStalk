// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityCharacterShade.h"

#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityCharacterMonster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "STK_EntityCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameElements/STK_TrapBase.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "ShadowStalk/Controllers/STK_EntityCharacterShadeController.h"
#include "ShadowStalk/Gamemodes/STK_MatchGameMode.h"

//Eyes
#include "Components/RectLightComponent.h"
#include "STK_EyeComponent.h"

//Pickups
#include "ShadowStalk/Pickups/STK_PickupBase.h"
#include "ShadowStalk/Pickups/STK_ItemBase.h"
#include "ShadowStalk/Inventory/STK_InventoryComponent.h"

//Sounds
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

#include "ShadowStalk/ExitDoor/STK_ExitDoor.h"

#include "Engine/TargetPoint.h"

// Sets default values
ASTK_EntityCharacterShade::ASTK_EntityCharacterShade()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SetupEyes();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASTK_EntityCharacterShade::OnBeginOverlap);

    //Default Shade Stats.
    m_MovementData.m_JumpStrength = 2000.0f;
    m_MovementData.m_Acceleration = 3500.0f;
    m_MovementData.m_WalkSpeed = 600.0f;
    m_MovementData.m_SprintSpeed = 800.0f;
    m_MovementData.m_CrawlSpeed = 200.0f;
    m_MovementData.m_CapsuleHalfHeight = 75.0f;
    m_MovementData.m_CrawlCapsuleHalfHeight = 50.f;
    m_MovementData.m_CapsuleRadius = 40.0f;

    GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

    Tags.Add("Shade");

    SetReplicates(true);
}

// Called when the game starts or when spawned
void ASTK_EntityCharacterShade::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocallyControlled())
    {
        m_pLSpotlight->SetVisibility(false);
        m_pRSpotlight->SetVisibility(false);
    }
    else
    {
        m_pFPSpotlight->SetVisibility(false);
        m_pEyeSocket->SetVisibility(false);
    }

    if (IsLocallyControlled())
    {
        GetController<ASTK_EntityCharacterShadeController>()->SetInputMode(FInputModeGameOnly());
        GetController<ASTK_EntityCharacterShadeController>()->bShowMouseCursor = false;
    }
}

/// <summary>
/// If we're overlapping another actor (likely because we were downed and are recovering), push out from the other collider and then enable collisions.
/// </summary>
void ASTK_EntityCharacterShade::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OverlappingAnotherEntity)
    {
        TArray<AActor*> OverlappingActors;
        GetOverlappingActors(OverlappingActors, ASTK_EntityCharacter::StaticClass());

        for (size_t i = 0; i < OverlappingActors.Num(); i++)
        {
            FVector direction = GetActorLocation() - OverlappingActors[i]->GetActorLocation();
            direction.Z = 0; direction.Normalize();
            GetCapsuleComponent()->AddImpulse(direction * 30000);
        }

        OverlappingAnotherEntity = OverlappingActors.Num() == 0;

    }

    if (BlinkPercentage != BlinkTarget)
    {
        // if BlinkTarget == 1 ? 1 : -1
        BlinkPercentage += 2 * (BlinkTarget - 0.5f) * DeltaTime * BlinkSpeed;
        BlinkPercentage = FMath::Clamp(BlinkPercentage, 0.0f, 1.0f);

        m_pEyes->SetState("Close", BlinkPercentage);

        if (BlinkPercentage == 1)
        {
            m_pLSpotlight->SetIntensity(0);
            m_pRSpotlight->SetIntensity(0);
            m_pFPSpotlight->SetIntensity(0);
        }
        else
        {
            m_pLSpotlight->SetIntensity((1 - BlinkPercentage) * InitBrightness);
            m_pRSpotlight->SetIntensity((1 - BlinkPercentage) * InitBrightness);
            m_pFPSpotlight->SetIntensity((1 - BlinkPercentage) * InitBrightness);
        }
    }
}

/// <summary>
/// prep the shade position and rotation for execution animation.
/// </summary>
void ASTK_EntityCharacterShade::StartExecution(ASTK_EntityCharacterMonster* Executioner)
{
    Server_StartExecution(Executioner);
}

void ASTK_EntityCharacterShade::Server_StartExecution_Implementation(ASTK_EntityCharacterMonster* Executioner)
{
    SetShadeState(ECharacterShadeState::Execution);
    LockCameraLookat(Executioner->m_CameraComp);

    //FVector targetPos = Executioner->GetActorLocation() + (GetActorLocation() - Executioner->GetActorLocation()).GetSafeNormal() * Executioner->ExecutionPositioningDistance;
    //targetPos.Z = GetCapsuleComponent()->GetRelativeLocation().Z;
    //ForceMoveToPoint(targetPos);

    DelayedTargetState = ECharacterShadeState::Dead;
    GetWorldTimerManager().SetTimer(DelayedStateChangeHandle, this, &ASTK_EntityCharacterShade::DelayedStateChange, Executioner->ExecutionTimeLength, false);
}

/// <summary>
/// Apply damage and set shade state accordingly. Knocks shade back if requested.
/// </summary>
void ASTK_EntityCharacterShade::ApplyDamage(unsigned char damage, FVector knockback)
{
    Health = FMath::Clamp(Health - damage, 0, 2);

    if (!knockback.IsNearlyZero())
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
        GetCharacterMovement()->AddImpulse(knockback + FVector(0, 0, 50000));
        GetWorldTimerManager().SetTimer(DelayedStateChangeHandle, this, &ASTK_EntityCharacterShade::DelayedStateChange, KnockbackRecoveryDuration, false);
    }

    switch (Health)
    {
    case 0:
        DelayedTargetState = ECharacterShadeState::Downed;
        SetShadeState(ECharacterShadeState::KnockedBack);
        break;

    case 1:
        DelayedTargetState = ECharacterShadeState::Hurt;
        break;

    default:
        DelayedTargetState = ECharacterShadeState::Default;
        break;
    }
}

/// <summary>
/// Helper function to apply a state change with a delay.
/// </summary>
void ASTK_EntityCharacterShade::DelayedStateChange()
{
    SetShadeState(DelayedTargetState);
}

//void ASTK_EntityCharacterShade::Client_Spectate_Implementation()
//{
//    Server_Spectate();
//
//    GetMesh()->DetachFromParent(true);
//
//    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString("Client"));
//
//    GetCapsuleComponent()->SetSimulatePhysics(false);
//    GetCapsuleComponent()->SetCollisionProfileName("Spectator");
//    SetInputLock(EInputLockFlags::Everything, true);
//    //SetInputLock(EInputLockFlags::Everything & !(EInputLockFlags::Movement | EInputLockFlags::MouseLook), true);
//}

int ASTK_EntityCharacterShade::GetHealth()
{
    return Health;
}


void ASTK_EntityCharacterShade::SetHealth(int targetHealth)
{
    Health = FMath::Clamp(targetHealth, 0, 2);
}


/// <summary>
/// Returns this shade's state
/// </summary>
ECharacterShadeState ASTK_EntityCharacterShade::GetShadeState()
{
    return CurrentState;

}

// void ASTK_EntityCharacterShade::Server_Spectate_Implementation()
// {
//     GetCapsuleComponent()->SetSimulatePhysics(false);
//     GetCapsuleComponent()->SetCollisionProfileName("Spectator");
//     SetInputLock(EInputLockFlags::Everything, true);
//     GetCharacterMovement()->bCheatFlying = true;
//     //SetInputLock(EInputLockFlags::Everything & !(EInputLockFlags::Movement | EInputLockFlags::MouseLook), true);
//     //GetCharacterMovement()->bCheatFlying = true;
// }

/// <summary>
/// Interact with the environment.
/// </summary>
void ASTK_EntityCharacterShade::Interact()
{
    if (InputLockFlags & EInputLockFlags::Interact)
        return;

    SetInputLock(EInputLockFlags::Everything, true);
}

void ASTK_EntityCharacterShade::Blink(bool blink)
{
    Server_Blink(blink);
}

void ASTK_EntityCharacterShade::Server_Blink_Implementation(bool blink)
{
    BlinkTarget = blink ? 1 : 0;
}

//void ASTK_EntityCharacterShade::HandleBlinkInput(float DeltaTime)
//{
//	// Server_HandleBlinkInput(DeltaTime);
//}
//
//void ASTK_EntityCharacterShade::Server_HandleBlinkInput_Implementation(float DeltaTime)
//{
//	//BlinkPercentage = BlinkTarget - (BlinkTarget - BlinkPercentage) * DeltaTime * BlinkSpeed;
//	//BlinkTarget ? BlinkPercentage += DeltaTime * BlinkSpeed : BlinkPercentage -= DeltaTime * BlinkSpeed;
//	//BlinkPercentage = FMath::Max(FMath::Clamp(BlinkPercentage, 0.0f, 1.0f), EyeClosedOverride);
//}

void ASTK_EntityCharacterShade::SetShadeState(ECharacterShadeState state)
{
    Server_SetShadeState(state);
}

/// <summary>
/// Sets the Shade's state. Also allows for custom functionality to apply when a certain state change occurs.
/// </summary>
void ASTK_EntityCharacterShade::Server_SetShadeState_Implementation(ECharacterShadeState state)
{
    switch (state)
    {

    case ECharacterShadeState::Hurt:
        // TODO: Apply cracked eye effects

        break;

    case ECharacterShadeState::Downed:

        Crawl(true);

        //Play the sound
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShadeDownGroundHit, GetActorLocation());

        // Lock everything but keep mouselook and blinking
        SetInputLock(EInputLockFlags::Everything & ~(EInputLockFlags::MouseLook | EInputLockFlags::Blink), true);

        GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
        GetWorldTimerManager().SetTimer(DownedRecoveryHandle, this, &ASTK_EntityCharacterShade::RecoverFromDowned, DownedRecoveryTime, false);

        break;

    case ECharacterShadeState::Dead:

        GetCapsuleComponent()->SetSimulatePhysics(false);
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        SetInputLock(EInputLockFlags::Everything, true);
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString("HE'S DEAD, JIM!"));
        GetWorldTimerManager().ClearAllTimersForObject(this);

       // Client_Spectate();
       // GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString("HE'S DEAD, JIM!"));
       // GetWorldTimerManager().ClearAllTimersForObject(this);

        // TODO: ragdoll
        // TODO: change eye material brightness to 0

        break;

    case ECharacterShadeState::Execution:
        SetInputLock(EInputLockFlags::Everything, true);
        break;

    default:
        // GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("UNDEFINED SHADE STATE!"));
        break;

    case ECharacterShadeState::Stuck:
        SetInputLock((EInputLockFlags::Movement | EInputLockFlags::Jump) & ~(EInputLockFlags::MouseLook | EInputLockFlags::Blink), true);
        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString("HELP, I'M STUCK!"));
        GetWorldTimerManager().SetTimer(StuckRecoveryHandle, this, &ASTK_EntityCharacterShade::RecoverFromTrap, StuckRecoveryTime, false);
        break;
    }

    // override any internal input locks here.

    CurrentState = state;
}


EEntityType ASTK_EntityCharacterShade::GetEntityType()
{
    return EEntityType::Shade;
}


/// <summary>
/// Get back up if downed
/// </summary>
void ASTK_EntityCharacterShade::RecoverFromDowned()
{
    if (GetShadeState() == ECharacterShadeState::Downed)
    {
        // TArray<ASTK_EntityCharacter*> entities = Cast<ASTK_MatchGameState>(GetWorld()->GetGameState())->GetEntities();
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
        SafeActivatePawnCollision();

        SetInputLock(EInputLockFlags::Everything, false);
        SetShadeState(ECharacterShadeState::Hurt);
        Crawl(false);
    }
}

void ASTK_EntityCharacterShade::RecoverFromTrap()
{
    if (GetShadeState() == ECharacterShadeState::Stuck)
    {
        SetInputLock(EInputLockFlags::Everything, false);
        SetShadeState(ECharacterShadeState::Hurt);
        //TODO Define Default State
    }
}

void ASTK_EntityCharacterShade::SafeActivatePawnCollision()
{
    bool success = false;

    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, ASTK_EntityCharacter::StaticClass());

    if (OverlappingActors.Num() == 0)
    {
        GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
        success = true;
    }

    if (!success)
    {
        OverlappingAnotherEntity = true;
        GetWorldTimerManager().SetTimer(SafeActivatePawnCollisionHandle, this, &ASTK_EntityCharacterShade::SafeActivatePawnCollision, 0.2f, false);
    }
}



/// <summary>
/// Handle overlap with other components.
/// </summary>
void ASTK_EntityCharacterShade::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Handle pickups
    if (OtherActor->ActorHasTag("Pickup"))
    {
        //TODO Find item pickup sound effect
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShadeItemPickupSound, GetActorLocation());

        EPickupType pickupType = Cast<ASTK_PickupBase>(OtherActor)->GetPickupType();
        switch (pickupType)
        {

        case EPickupType::Undefined:
        {
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("UNDEFINED PICKUP TYPE!"));
            break;
        }

        case EPickupType::Key:
        {
            // TODO: ADD THE PICKED ITEM TO PLAYERSTATE INVENTORY.
            GetWorld()->GetGameState<ASTK_MatchGameState>()->Register_KeyPickedUp();
            OtherActor->Destroy();
            break;
        }

        case EPickupType::Item:
        {
            EItemType itemType = Cast<ASTK_ItemBase>(OtherActor)->GetItemType();
            switch (itemType)
            {
                case EItemType::TestItem1:
                {
                    // TODO: ADD THE PICKED ITEM TO PLAYERSTATE INVENTORY.
                    InventoryComponent->AddToInventory(Cast<ASTK_PickupBase>(OtherActor));
                    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString("Added TestItem1"));
                    OtherActor->Destroy();
                    break;
                }

                case EItemType::TestItem2:
                {
                    // TODO: ADD THE PICKED ITEM TO PLAYERSTATE INVENTORY.
                    InventoryComponent->AddToInventory(Cast<ASTK_PickupBase>(OtherActor));
                    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, FString("Added TestItem2"));
                    OtherActor->Destroy();
                    break;
                }
            }
        }

        }
    }

    else if (OtherActor->ActorHasTag("Trap"))
    {
        Cast<ASTK_TrapBase>(OtherActor)->ActivateTrap();
        SetShadeState(ECharacterShadeState::Stuck);
    }

    else if (OtherActor->ActorHasTag("Door"))
    {
        if (CurrentState == ECharacterShadeState::Dead)
        {
            return;
        }

        if (Cast<ASTK_ExitDoor>(OtherActor)->GetIsOpen())
        {
            // SHADE WIN STATE

            FString name = GetName();
            TArray< FStringFormatArg > args;
            args.Add(FStringFormatArg(name));
            FString string = FString::Format(TEXT("{0} Has gotten away with it lmao"), args);
            if (HasAuthority())
            {
                GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, string);
            }

            TArray<AActor*> found;
            UGameplayStatics::GetAllActorsOfClass( GetWorld(), ATargetPoint::StaticClass(), found );

            for (size_t i = 0; i < found.Num(); i++)
            {
                ATargetPoint* t = Cast< ATargetPoint >(found[i]);
                if (t)
                {
                    if (t->ActorHasTag("EndRoom"))
                    {
                        UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShadeTeleportSound, GetActorLocation());
                        if (GetController())
                        {
                            GetController()->SetControlRotation(t->GetActorRotation());
                        }

                        GetRootComponent()->SetRelativeLocation(t->GetActorLocation());

                        break;
                    };
                }
            }
        }
    }
}


/// <summary>
/// Sets up the lights and their positioning, as well as the blinking component. Numbers hard-coded to ensure no loss of effort in case of a blueprint reset.
/// </summary>
void ASTK_EntityCharacterShade::SetupEyes()
{
    m_pEyeSocket = CreateDefaultSubobject<USkeletalMeshComponent>("m_pEyeSocket");
    m_pEyeSocket->SetupAttachment(m_CameraComp);
    m_pEyeSocket->CastShadow = 0;

    InitBrightness = 12000.0f;

    m_pEyes = CreateDefaultSubobject<USTK_EyeComponent>(TEXT("Eyes"));
    m_pEyes->SetupTPMesh(m_TPMeshComp);
    m_pEyes->SetupFPMesh(m_pEyeSocket);

    m_pLSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pLSpotlight"));
    m_pRSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pRSpotlight"));

    m_pLSpotlight->SetRelativeLocation(FVector(-4.358307, 1.676558, -7.412248));
    m_pLSpotlight->SetRelativeRotation(FRotator(79.999390, 269.600494, 360.000641));

    m_pRSpotlight->SetRelativeLocation(FVector(4.358307, 1.676558, -7.412248));
    m_pRSpotlight->SetRelativeRotation(FRotator(79.999390, 269.600494, 360.000641));

    m_pRSpotlight->Intensity = InitBrightness;
    m_pLSpotlight->Intensity = InitBrightness;

    m_pRSpotlight->AttenuationRadius = 2500.f;
    m_pLSpotlight->AttenuationRadius = 2500.f;

    m_pRSpotlight->SourceWidth = 4.823f;
    m_pLSpotlight->SourceWidth = 4.823f;

    m_pRSpotlight->SourceHeight = 9.f;
    m_pLSpotlight->SourceHeight = 9.f;

    m_pRSpotlight->BarnDoorAngle = 0;
    m_pLSpotlight->BarnDoorAngle = 0;

    m_pRSpotlight->BarnDoorLength = 12.f;
    m_pLSpotlight->BarnDoorLength = 12.f;

    m_pRSpotlight->VolumetricScatteringIntensity = 0.01f;
    m_pLSpotlight->VolumetricScatteringIntensity = 0.01f;

    m_pLSpotlight->AttachToComponent(m_TPMeshComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
    m_pRSpotlight->AttachToComponent(m_TPMeshComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));

    // First person light

    m_pFPSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pFPSpotlight"));

    m_pFPSpotlight->Intensity = InitBrightness;
    m_pFPSpotlight->AttenuationRadius = 2500.f;
    m_pFPSpotlight->SourceWidth = 9.6f;
    m_pFPSpotlight->SourceHeight = 9.f;
    m_pFPSpotlight->BarnDoorAngle = 0;
    m_pFPSpotlight->BarnDoorLength = 12.f;
    m_pFPSpotlight->VolumetricScatteringIntensity = 0.01f;
    m_pFPSpotlight->SetupAttachment(m_CameraComp);


}

void ASTK_EntityCharacterShade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASTK_EntityCharacterShade, BlinkTarget);
    DOREPLIFETIME(ASTK_EntityCharacterShade, CurrentState);
}

// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityShade.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityMonster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "STK_Entity.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowStalk/GameElements/STK_TrapBase.h"

//Eyes
#include "Components/RectLightComponent.h"
#include "STK_EyeComponent.h"

//Pickups
#include "ShadowStalk/Pickups/STK_PickupBase.h"

//Sounds
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"


// Sets default values
ASTK_EntityShade::ASTK_EntityShade()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupEyes();

	m_PlayerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASTK_EntityShade::OnBeginOverlap);

	//Default Shade Stats. 
	m_MovementData.m_WalkSpeed = 500.0f;
	m_MovementData.m_SprintSpeed = 800.0f;
	m_MovementData.m_CrawlSpeed = 200.0f;
	m_MovementData.m_Acceleration = 3500.0f;
	m_MovementData.m_JumpStrength = 20000.0f;
	m_MovementData.m_CapsuleHalfHeight = 75.0f;
	m_MovementData.m_CrawlCapsuleHalfHeight = 50.f;
	m_MovementData.m_CapsuleRadius = 40.0f;

	bReplicates = true;
	SetReplicates(true);
	SetReplicatingMovement(true);

}

// Called when the game starts or when spawned
void ASTK_EntityShade::BeginPlay()
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

	// here's how to use the eyes. each new emotion gets added to a queue to be emoted.
	// order is: Emotion name, Emotion intensity, Duration, Transition speed, and fidgeting intensity.

	//m_pEyes->SetEmotion("Close"	, 1.0f, 1, 2.0f);
	//m_pEyes->SetEmotion("Angry"	, 0.6f, 5, 2.0f);
	//m_pEyes->SetEmotion("Happy"	, 1.0f, 2, 2.0f);
	//m_pEyes->SetEmotion("Angry"	, 1.0f, 5, 2.0f);
	//m_pEyes->SetEmotion("Happy"	, 1.0f, 5, 2.0f);
	//m_pEyes->SetEmotion("Sad"	, 0.6f, 5, 2.0f);
	//m_pEyes->SetEmotion("Sad"	, 1.0f, 5, 2.0f);
	//m_pEyes->SetEmotion("Close"	, 0.5f, 1, 2.0f, 0.5f);
}

// Called every frame
void ASTK_EntityShade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OverlappingAnotherEntity)
	{
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors, ASTK_Entity::StaticClass());

		for (size_t i = 0; i < OverlappingActors.Num(); i++)
		{
			FVector direction = GetActorLocation() - OverlappingActors[i]->GetActorLocation();
			direction.Z = 0; direction.Normalize();
			m_PlayerCapsule->AddImpulse(direction * 30000);
		}

		OverlappingAnotherEntity = OverlappingActors.Num() == 0;
	}

	//if (BlinkPercentage != BlinkTarget)
	{
		HandleBlinkInput(DeltaTime);
		m_pEyes->SetState("Close", BlinkPercentage);

		if (BlinkPercentage == 1)
		{
			m_pLSpotlight ->SetIntensity(0);
			m_pRSpotlight ->SetIntensity(0);
			m_pFPSpotlight->SetIntensity(0);
		}
		else
		{
			m_pLSpotlight ->SetIntensity( (1 - BlinkPercentage) * InitBrightness );
			m_pRSpotlight ->SetIntensity( (1 - BlinkPercentage) * InitBrightness );
			m_pFPSpotlight->SetIntensity( (1 - BlinkPercentage) * InitBrightness );
		}
	}
}

/// <summary>
/// prep the shade position and rotation for execution animation.
/// </summary>
void ASTK_EntityShade::StartExecution(ASTK_EntityMonster* Executioner)
{
	Server_StartExecution(Executioner);
}

void ASTK_EntityShade::Server_StartExecution_Implementation(ASTK_EntityMonster* Executioner)
{
	SetShadeState(EShadeState::Execution);
	//FVector x = Executioner->GetActorLocation() + Executioner->m_CameraComp->GetComponentLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("%f, %f, %f"), x.X,x.Y,x.Z));

	LockCameraLookat(Executioner->m_CameraComp->GetComponentLocation());

	FVector targetPos = Executioner->GetActorLocation() + (GetActorLocation() - Executioner->GetActorLocation()).GetSafeNormal() * Executioner->ExecutionPositioningDistance;
	targetPos.Z = m_PlayerCapsule->GetRelativeLocation().Z;
	ForceMoveToPoint(targetPos);
	//m_PlayerCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//m_PlayerCapsule->SetEnableGravity(false);

	DelayedTargetState = EShadeState::Dead;
	GetWorldTimerManager().SetTimer(DelayedStateChangeHandle, this, &ASTK_EntityShade::DelayedStateChange, Executioner->ExecutionTimeLength, false);
}

/// <summary>
/// Apply damage and set shade state accordingly. Knocks shade back if requested.
/// </summary>
void ASTK_EntityShade::ApplyDamage(unsigned char damage, FVector knockback)
{
	Health = FMath::Clamp(Health - damage, 0, 2);

	if (!knockback.IsNearlyZero())
	{
		Jump();

		if (GetRootComponent()->IsSimulatingPhysics())
		{
			m_PlayerCapsule->AddImpulse(knockback);
		}

		GetWorldTimerManager().SetTimer(DelayedStateChangeHandle, this, &ASTK_EntityShade::DelayedStateChange, KnockbackRecoveryDuration, false);
	}

	switch (Health)
	{
		case 0:
			DelayedTargetState = EShadeState::Downed;
			SetShadeState(EShadeState::KnockedBack);
			break;

	case 1:
		DelayedTargetState = EShadeState::Hurt;
		break;

	default:
		DelayedTargetState = EShadeState::Default;
		break;
	}
}

/// <summary>
/// Helper function to apply a state change with a delay.
/// </summary>
void ASTK_EntityShade::DelayedStateChange()
{
	SetShadeState(DelayedTargetState);
}


int ASTK_EntityShade::GetHealth()
{
	return Health;
}


void ASTK_EntityShade::SetHealth(int targetHealth)
{
	Health = FMath::Clamp(targetHealth, 0, 2);
}


/// <summary>
/// Returns this shade's state
/// </summary>
EShadeState ASTK_EntityShade::GetShadeState()
{
	return CurrentState;
}


/// <summary>
/// Interact with the environment.
/// </summary>
void ASTK_EntityShade::Interact()
{
	if (InputLockFlags & EInputLockFlags::Interact)
		return;
}

void ASTK_EntityShade::CloseEyes()
{
	Server_CloseEyes();
}

void ASTK_EntityShade::Server_CloseEyes_Implementation()
{
	BlinkTarget = 1;
}

void ASTK_EntityShade::OpenEyes()
{
	Server_OpenEyes();
}

void ASTK_EntityShade::Server_OpenEyes_Implementation()
{
	BlinkTarget = 0;
}

void ASTK_EntityShade::HandleBlinkInput(float DeltaTime)
{
	Server_HandleBlinkInput(DeltaTime);
}

void ASTK_EntityShade::Server_HandleBlinkInput_Implementation(float DeltaTime)
{
	//BlinkPercentage = BlinkTarget - (BlinkTarget - BlinkPercentage) * DeltaTime * BlinkSpeed;
	BlinkTarget ? BlinkPercentage += DeltaTime * BlinkSpeed : BlinkPercentage -= DeltaTime * BlinkSpeed;
	BlinkPercentage = FMath::Max(FMath::Clamp(BlinkPercentage, 0.0f, 1.0f), EyeClosedOverride);
}

/// <summary>
/// Sets the Shade's state. Also allows for custom functionality to apply when a certain state change occurs.
/// </summary>
void ASTK_EntityShade::SetShadeState(EShadeState state)
{
	switch (state)
	{

		case EShadeState::Hurt:
			//TODO: Find proper place for this sound effect to play
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShadeHitScream, GetActorLocation());

			// TODO: Apply cracked eye effects
			break;

		case EShadeState::Downed:

			Crawl(true);

			//Play the sound
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShadeDownGroundHit, GetActorLocation());

			// Lock everything but keep mouselook and blinking
			SetInputLock(EInputLockFlags::Everything & ~(EInputLockFlags::MouseLook | EInputLockFlags::Blink), true);

			m_PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			GetWorldTimerManager().SetTimer(DownedRecoveryHandle, this, &ASTK_EntityShade::RecoverFromDowned, DownedRecoveryTime, false);

			break;

    case EShadeState::Dead:
			m_PlayerCapsule->SetSimulatePhysics(false);
			m_PlayerCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SetInputLock(EInputLockFlags::Everything, true);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString("HE'S DEAD, JIM!"));
			GetWorldTimerManager().ClearAllTimersForObject(this);
			// TODO: ragdoll
			// TODO: change eye material brightness to 0

			break;

		case EShadeState::Execution:
			SetInputLock(EInputLockFlags::Everything, true);
			break;

		default:
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString("UNDEFINED SHADE STATE!"));
			break;

		case EShadeState::Stuck:
			SetInputLock(EInputLockFlags::Movement & ~(EInputLockFlags::MouseLook | EInputLockFlags::Blink), true);
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString("HELP, I'M STUCK!"));
			GetWorldTimerManager().SetTimer(StuckRecoveryHandle, this, &ASTK_EntityShade::RecoverFromTrap, StuckRecoveryTime, false);
			break;
	}

	// override any internal input locks here.

	CurrentState = state;
}


EEntityType ASTK_EntityShade::GetEntityType()
{
	return EEntityType::Shade;
}


/// <summary>
/// Get back up if downed
/// </summary>
void ASTK_EntityShade::RecoverFromDowned()
{
  if (GetShadeState() == EShadeState::Downed)
	{
		// TArray<ASTK_Entity*> entities = Cast<ASTK_MatchGameState>(GetWorld()->GetGameState())->GetEntities();
		m_PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		SafeActivatePawnCollision();

		SetInputLock(EInputLockFlags::Crawl, false);
		SetShadeState(EShadeState::Hurt);
		Crawl(false);
	}
}

void ASTK_EntityShade::RecoverFromTrap()
{
	if (GetShadeState() == EShadeState::Stuck)
	{
		SetInputLock(EInputLockFlags::Everything, false);
		SetShadeState(EShadeState::Default);
		//TODO Define Default State
  }	
}

void ASTK_EntityShade::SafeActivatePawnCollision()
{
	bool success = false;

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, ASTK_Entity::StaticClass());

	if (OverlappingActors.Num() == 0)
	{
		m_PlayerCapsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		success = true;
	}

	if (!success)
	{
		OverlappingAnotherEntity = true;
		GetWorldTimerManager().SetTimer(SafeActivatePawnCollisionHandle, this, &ASTK_EntityShade::SafeActivatePawnCollision, 0.2f, false);
	}
}



/// <summary>
/// Handle overlap with other components.
/// </summary>
void ASTK_EntityShade::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Handle pickups
	if (OtherActor->ActorHasTag("Pickup"))
	{
		//TODO Find item pickup sound effect
		//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShadeItemPickupSound, GetActorLocation());

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
				// TODO: ADD THE PICKED ITEM TO PLAYERSTATE INVENTORY.
				OtherActor->Destroy();
				break;
			}
		}
	}

	else if (OtherActor->ActorHasTag("Trap"))
	{
		Cast<ASTK_TrapBase>(OtherActor)->ActivateTrap();
		SetShadeState(EShadeState::Stuck);
	}
}


/// <summary>
/// Sets up the lights and their positioning, as well as the blinking component. Numbers hard-coded to ensure no loss of effort in case of a blueprint reset.
/// </summary>
void ASTK_EntityShade::SetupEyes()
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

void ASTK_EntityShade::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASTK_EntityShade, BlinkPercentage);
	DOREPLIFETIME(ASTK_EntityShade, EyeClosedOverride);
}

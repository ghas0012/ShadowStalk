// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_EntityShade.h"
#include "ShadowStalk/Gamestates/STK_MatchGameState.h"
#include "ShadowStalk/Entity/STK_EntityMonster.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "STK_Entity.h"
#include "Kismet/GameplayStatics.h"

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
	m_WalkSpeed = 500.0f;
	m_SprintSpeed = 800.0f;
	m_CrawlSpeed = 200.0f;
	m_Acceleration = 3500.0f;
	m_JumpStrength = 20000.0f;
	m_CapsuleHalfHeight = 75.0f;
	m_CrawlCapsuleHalfHeight = 50.f;
	m_CapsuleRadius = 40.0f;
}

// Called when the game starts or when spawned
void ASTK_EntityShade::BeginPlay()
{
	Super::BeginPlay();

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
}

/// <summary>
/// prep the shade position and rotation for execution animation.
/// </summary>
void ASTK_EntityShade::StartExecution(ASTK_EntityMonster* Executioner)
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
}


/// <summary>
/// Sets up the lights and their positioning, as well as the blinking component. Numbers hard-coded to ensure no loss of effort in case of a blueprint reset.
/// </summary>
void ASTK_EntityShade::SetupEyes()
{
	m_pEyes = CreateDefaultSubobject<USTK_EyeComponent>(TEXT("Eyes"));
	m_pEyes->SetupMesh(m_MeshComp);

	m_pLSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pLSpotlight"));
	m_pRSpotlight = CreateDefaultSubobject<URectLightComponent>(TEXT("m_pRSpotlight"));

	m_pLSpotlight->SetRelativeLocation(FVector(-4.358307, 1.676558, -7.412248));
	m_pLSpotlight->SetRelativeRotation(FRotator(79.999390, 269.600494, 360.000641));

	m_pRSpotlight->SetRelativeLocation(FVector(4.358307, 1.676558, -7.412248));
	m_pRSpotlight->SetRelativeRotation(FRotator(79.999390, 269.600494, 360.000641));

	m_pRSpotlight->Intensity = 12000.f;
	m_pLSpotlight->Intensity = 12000.f;

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

	m_pLSpotlight->AttachToComponent(m_MeshComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
	m_pRSpotlight->AttachToComponent(m_MeshComp, FAttachmentTransformRules::KeepRelativeTransform, TEXT("headSocket"));
}
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
#include "Net/UnrealNetwork.h"

// Sets default values
ASTK_Entity::ASTK_Entity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PlayerCapsule = CreateDefaultSubobject<UCapsuleComponent>("Player Capsule");
	m_PlayerCapsule->SetCapsuleRadius(m_CapsuleRadius);
	m_PlayerCapsule->SetCapsuleHalfHeight(m_CapsuleHalfHeight);
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

	SetReplicates(true);
	SetReplicatingMovement(true);
}

// Called when the game starts or when spawned
void ASTK_Entity::BeginPlay()
{

	m_MovementComp->WalkSpeed = m_WalkSpeed;
	m_MovementComp->AirControl = m_AirControl;
	m_MovementComp->SprintSpeed = m_SprintSpeed;
	m_MovementComp->CurrentSpeed = m_WalkSpeed;
	m_MovementComp->Acceleration = m_Acceleration;
	m_MovementComp->FrictionLerp = m_FrictionLerp;
	m_MovementComp->CapsuleCrawlHalfHeight = m_CrawlCapsuleHalfHeight;
	m_MovementComp->CapsuleStandingHalfHeight = m_CapsuleHalfHeight;

	m_MovementComp->CrawlSpeed = m_CrawlSpeed;

	Super::BeginPlay();

	MouseLookVector.Y = m_CameraComp->GetRelativeRotation().Pitch;
	MouseLookVector.X = m_PlayerCapsule->GetRelativeRotation().Yaw;

	//TODO - GameState callers.
}

void ASTK_Entity::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, FString::Printf(TEXT("%d"), GetWorld()->GetNumPlayerControllers()));
	
}

void ASTK_Entity::Restart()
{
	Super::Restart();
}

void ASTK_Entity::HandleCamera(float DeltaTime)
{
	if (!bCameraOverride)
	{

		m_PlayerCapsule->SetRelativeRotation(FRotator(0, MouseLookVector.X, 0));

		if (abs(MouseLookVector.Y) < m_MouseLook_VerticalLookLimitAngle)
		{
			m_CameraComp->SetRelativeRotation(FRotator(MouseLookVector.Y, 0, 0));
		}
		else
		{
			MouseLookVector.Y = -m_MouseLook_VerticalLookLimitAngle * (signbit(MouseLookVector.Y) * 2 - 1);
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

void ASTK_Entity::Forward(float value)
{
	Server_Forward(value);
}

void ASTK_Entity::Server_Forward_Implementation(float value)
{
	if (InputLockFlags & EInputLockFlags::Movement || bPositionOverride)
	{
		RightAccelerationVector = FVector(0);
		return;
	}

	ForwardAccelerationVector = GetActorForwardVector() * value;
	//AddMovementInput(GetActorForwardVector(), value);
}

bool ASTK_Entity::Server_Forward_Validate(float value)
{
	return true;
}

void ASTK_Entity::Strafe(float value)
{
	Server_Strafe(value);
}

void ASTK_Entity::Server_Strafe_Implementation(float value)
{
	if (InputLockFlags & EInputLockFlags::Movement || bPositionOverride)
	{
		RightAccelerationVector = FVector(0);
		return;
	}

	RightAccelerationVector = GetActorRightVector() * value;
}

bool ASTK_Entity::Server_Strafe_Validate(float value)
{
	return true;
}

void ASTK_Entity::LockCameraLookat(FVector Location)
{
	bCameraOverride = true;
	CameraOverrideTarget = Location;
}

void ASTK_Entity::UnlockCameraLookat()
{
	bCameraOverride = false;
}

void ASTK_Entity::ForceMoveToPoint(FVector target)
{
	bPositionOverride = true;
	PositionOverrideTarget = target;
	PositionOverrideOrigin = m_PlayerCapsule->GetRelativeLocation();
}

void ASTK_Entity::Interact()
{
	// LEAVE EMPTY, Will get overrided by child classes.
}

void ASTK_Entity::Jump()
{
	Server_Jump();
}

void ASTK_Entity::Server_Jump_Implementation()
{
	if (InputLockFlags & EInputLockFlags::Jump)
		return;

	if (m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
	{
		if (m_MovementComp->bIsGrounded)
		{
			m_MovementComp->Jump(m_JumpStrength);
			m_MovementComp->bIsGrounded = false;
		}
	}
}

bool ASTK_Entity::Server_Jump_Validate()
{
	return true;
}

void ASTK_Entity::Sprint(bool IsSprint)
{
	Server_Sprint(IsSprint);
}

void ASTK_Entity::Server_Sprint_Implementation(bool IsSprint)
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

bool ASTK_Entity::Server_Sprint_Validate(bool IsSprint)
{
	return true;
}

void ASTK_Entity::Crawl(bool IsCrawl)
{	
	Server_Crawl(IsCrawl);
}

void ASTK_Entity::Server_Crawl_Implementation(bool IsCrawl)
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

bool ASTK_Entity::Server_Crawl_Validate(bool IsCrawl)
{
	return true;
}

void ASTK_Entity::UnhideMouse()
{
	//TODO - Have to create Controller (might have to be put in Child classes) // AMENDMENT: ARI SAYS SHE'LL TAKE CARE OF THIS
}

void ASTK_Entity::HideMouse()
{
	//TODO - Have to create Controller (might have to be put in Child classes) // AMENDMENT: ARI SAYS SHE'LL TAKE CARE OF THIS
}

void ASTK_Entity::MouseLook_Vertical(float value)
{
	Server_MouseLook_Vertical(value);
}

void ASTK_Entity::Server_MouseLook_Vertical_Implementation(float value)
{
	if (InputLockFlags & EInputLockFlags::MouseLook)
		return;

	MouseLookVector.Y += m_MouseLook_Y * value;
	//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouselook Y : %f "), MouseLookVector.Y));
}

bool ASTK_Entity::Server_MouseLook_Vertical_Validate(float value)
{
	return true;
}

void ASTK_Entity::MouseLook_Horizontal(float value)
{
	Server_MouseLook_Horizontal(value);
}

void ASTK_Entity::Server_MouseLook_Horizontal_Implementation(float value)
{
	if (InputLockFlags & EInputLockFlags::MouseLook)
		return;

	MouseLookVector.X += m_MouseLook_X * value;
	//GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouselook X : %f "), MouseLookVector.X));
}

bool ASTK_Entity::Server_MouseLook_Horizontal_Validate(float value)
{
	return true;
}

bool ASTK_Entity::GetIsGrounded()
{
	return bIsGrounded;
}

EEntityType ASTK_Entity::GetEntityType()
{
	return EEntityType::Undefined;
}

// Called every frame
void ASTK_Entity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleCamera(DeltaTime);
	HandleFootstepSounds(DeltaTime);
	HandlePosition(DeltaTime);
}


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

// Called to bind functionality to input
void ASTK_Entity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASTK_Entity::SetInputLock(EInputLockFlags flag, bool lock)
{
	SetInputLock(static_cast<uint8>(flag), lock);
}

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

void ASTK_Entity::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASTK_Entity, m_MovementComp);
	DOREPLIFETIME(ASTK_Entity, MouseLookVector);

}
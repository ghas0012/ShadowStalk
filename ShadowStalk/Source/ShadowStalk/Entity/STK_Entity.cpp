// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young

// Changelog:
// - Class init.
// - Changed AttachTo to SetupAttachment

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
	m_CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
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

	m_MovementComp->FrictionLerp = m_FrictionLerp;
	m_MovementComp->Acceleration = m_Acceleration;
	m_MovementComp->WalkSpeed = m_MaxWalkSpeed;
	m_MovementComp->SprintSpeed = m_MaxSprintSpeed;
	m_MovementComp->CrawlSpeed = m_MaxCrawlSpeed;
	m_MovementComp->AirControl = m_AirControl;
	m_MovementComp->CurrentSpeed = m_MaxWalkSpeed;

	Super::BeginPlay();

	MouseLookVector.Y = m_CameraComp->GetRelativeRotation().Pitch;
	MouseLookVector.X = m_PlayerCapsule->GetRelativeRotation().Yaw;

	//TODO - GameState callers.
}

void ASTK_Entity::HandleCamera()
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

void ASTK_Entity::Forward(float value)
{

	ForwardAccelerationVector = GetActorForwardVector() * value;
	//AddMovementInput(GetActorForwardVector(), value);
}

void ASTK_Entity::Strafe(float value)
{
	RightAccelerationVector = GetActorRightVector() * value;
}

void ASTK_Entity::Interact()
{
	//LEAVE EMPTY, Will get overrided by child classes.
}

void ASTK_Entity::Jump()
{
	if (m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
	{
		if (m_MovementComp->bIsGrounded)
		{
			m_MovementComp->Jump(m_JumpStrength);
			m_MovementComp->bIsGrounded = false;
		}
	}
}

void ASTK_Entity::Sprint(bool IsSprint)
{
	if (IsSprint == true)
	{
		m_MovementComp->Sprint();
	}

	else
	{
		m_MovementComp->Walk();
	}
}

void ASTK_Entity::Crawl(bool IsCrawl)
{
	if (IsCrawl == true)
	{

		m_MovementComp->Crawl();
	}
	if (IsCrawl == false)
	{
		m_MovementComp->Walk();
	}
}

void ASTK_Entity::UnhideMouse()
{
	//TODO - Have to create Controller (might have to be put in Child classes)
}

void ASTK_Entity::HideMouse()
{
	//TODO - Have to create Controller (might have to be put in Child classes)
}

void ASTK_Entity::MouseLook_Vertical(float value)
{
	MouseLookVector.Y += m_MouseLook_Y * value;
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouselook Y : %f "), MouseLookVector.Y));

}

void ASTK_Entity::MouseLook_Horizontal(float value)
{
	MouseLookVector.X += m_MouseLook_X * value;
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Orange, FString::Printf(TEXT("Mouselook X : %f "), MouseLookVector.X));

}

bool ASTK_Entity::GetIsGrounded()
{
	return bIsGrounded;
}

// Called every frame
void ASTK_Entity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleCamera();

	//TODO Make Footsteps it's own function.
	FootstepTimer += DeltaTime * FootstepFrequency * m_MovementComp->GetForwardVelocity();
	if (m_MovementComp->GetIsGrounded() && FootstepTimer >= 1 && !AudioComponent->IsPlaying())
	{
		FootstepTimer = 0;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, TEXT("Footstep"));

		//AudioComponent->SetSound(FootstepsSound);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepsSound, GetActorLocation());
	}


	//TODO Put this stuff in it's own function.
	FVector CombinedAccleration = ForwardAccelerationVector + RightAccelerationVector;
	if (!CombinedAccleration.IsNearlyZero() && m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
	{
		m_MovementComp->AddInputVector(CombinedAccleration);
	}
}

// Called to bind functionality to input
void ASTK_Entity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

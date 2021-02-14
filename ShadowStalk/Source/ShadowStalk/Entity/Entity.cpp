// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../MovementComponent/EntityMovementComponent.h"

// Sets default values
AEntity::AEntity()
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
	m_PlayerCapsule->SetCollisionProfileName("BlockAll");
	SetRootComponent(m_PlayerCapsule);

	m_InteractComp = CreateDefaultSubobject<USphereComponent>("Interact Comp");
	m_InteractComp->SetSphereRadius(m_InteractRadius);
	m_InteractComp->SetEnableGravity(false);
	m_InteractComp->SetSimulatePhysics(false);
	m_InteractComp->SetCollisionProfileName("OverlapAll");
	m_InteractComp->SetupAttachment(m_PlayerCapsule);

	m_CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Comp");
	m_CameraComp->SetProjectionMode(ECameraProjectionMode::Perspective);
	m_CameraComp->SetupAttachment(m_PlayerCapsule);

	m_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh Comp");
	m_MeshComp->SetupAttachment(m_PlayerCapsule);
	m_MeshComp->bHiddenInGame = false;

	m_MovementComp = CreateDefaultSubobject<UEntityMovementComponent>("Movement Component");
	m_MovementComp->CapsuleComp = m_PlayerCapsule;
	m_MovementComp->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void AEntity::BeginPlay()
{

	m_MovementComp->FrictionLerp = m_FrictionLerp;
	m_MovementComp->Acceleration = m_Acceleration;
	m_MovementComp->SprintSpeed = m_MaxWalkSpeed;
	m_MovementComp->AirControl = m_AirControl;

	Super::BeginPlay();
	//TODO - GameState callers.
}

void AEntity::HandleCamera()
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

void AEntity::Forward(float value)
{
	ForwardAccelerationVector = GetActorForwardVector() * value;
	ForwardInput = value;

}

void AEntity::Strafe(float value)
{
	RightAccelerationVector = GetActorRightVector() * value;
}

void AEntity::Interact()
{
	//LEAVE EMPTY, Will get overrided by child classes.
}

void AEntity::Jump()
{
	if (m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
	{
		if (m_MovementComp->bIsGrounded)
		{
			m_MovementComp->Jump(m_JumpStrength);
		}
	}
}

void AEntity::Crawl(bool IsCrawl)
{
	//TODO - Will Do Later.
}

void AEntity::UnhideMouse()
{
	//TODO - Have to create Controller (might have to be put in Child classes)
}

void AEntity::HideMouse()
{
	//TODO - Have to create Controller (might have to be put in Child classes)
}

void AEntity::MouseLook_Vertical(float value)
{
	MouseLookVector.Y += m_MouseLook_Y * value;
}

void AEntity::MouseLook_Horizontal(float value)
{
	MouseLookVector.X += m_MouseLook_X * value;
}

bool AEntity::GetIsGrounded()
{
	return bIsGrounded;
}

// Called every frame
void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleCamera();

	FVector CombinedAccleration = ForwardAccelerationVector + RightAccelerationVector;
	if (!CombinedAccleration.IsNearlyZero() && m_MovementComp && (m_MovementComp->UpdatedComponent == RootComponent))
	{
		m_MovementComp->AddInputVector(CombinedAccleration);
	}

}

// Called to bind functionality to input
void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

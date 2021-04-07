// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_ExitDoor.h"
#include "Components/BoxComponent.h"
#include "ShadowStalk/GameModes/STK_MatchGameMode.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"


ASTK_ExitDoor::ASTK_ExitDoor()
{
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	SetRootComponent(SceneComp);

	DoorCollider = CreateDefaultSubobject<UBoxComponent>("Player Capsule");
	DoorCollider->InitBoxExtent(FVector(80,20,120));
	DoorCollider->SetRelativeLocation(FVector(0, 0, 120));
	DoorCollider->SetCollisionProfileName("BlockAll");
	DoorCollider->SetupAttachment(SceneComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Comp");
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionProfileName("NoCollision");
	MeshComp->SetupAttachment(SceneComp);

	ParticleFX = CreateDefaultSubobject<UParticleSystemComponent>("Particles");
	ParticleFX->SetupAttachment(SceneComp);

	SetReplicates(true);
}


/// <summary>
/// Register the door with the game mode in order to open one randomly.
/// </summary>
void ASTK_ExitDoor::BeginPlay()
{
	ASTK_MatchGameMode* pGameMode = nullptr;

	if (GetWorld())
	{
		pGameMode = Cast<ASTK_MatchGameMode>(GetWorld()->GetAuthGameMode());
	}

	if (pGameMode)
	{
		pGameMode->RegisterPotentialExitDoor(this);
	}
}

void ASTK_ExitDoor::DoorOpen()
{
	if (HasAuthority())
	{
		NMC_DoorOpen();
	}
}

/// <summary>
/// Open the door.
/// </summary>
void ASTK_ExitDoor::NMC_DoorOpen_Implementation()
{
	ParticleFX->DeactivateSystem();
	DoorCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("DOOR IS OPEN!"));
}

void ASTK_ExitDoor::DoorClose()
{
	if (HasAuthority())
	{
		NMC_DoorClose();
	}
}

/// <summary>
/// Close the door.
/// </summary>
void ASTK_ExitDoor::NMC_DoorClose_Implementation()
{
	ParticleFX->ActivateSystem();
	DoorCollider->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("DOOR IS CLOSED!"));
}


bool ASTK_ExitDoor::GetIsOpen()
{
	return bIsOpen;
}


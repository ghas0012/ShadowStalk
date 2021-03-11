// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.

#include "STK_PickupBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASTK_PickupBase::ASTK_PickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>("Root");
	SceneRootComp->SetRelativeLocation(FVector(0));
	SetRootComponent(SceneRootComp);

	Collider = CreateDefaultSubobject<USphereComponent>("BoxCollider");
	Collider->InitSphereRadius(Radius);
	Collider->SetEnableGravity(false);
	Collider->SetGenerateOverlapEvents(true);
	Collider->SetCollisionProfileName("OverlapAll");
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetRelativeLocation(FVector(0, 0, 75));
	Collider->SetupAttachment(SceneRootComp);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetRelativeLocation(FVector(0, 0, -40));
	PickupMesh->SetupAttachment(Collider);

	ParticleFX = CreateDefaultSubobject<UParticleSystemComponent>("Particles");
	ParticleFX->SetRelativeLocation(FVector(0, 0, -75));
	ParticleFX->SetupAttachment(Collider);

	Tags.Add("Pickup"); // TODO maybe convert to "IsA..." in shade's on overlap function.
}

void ASTK_PickupBase::Enable()
{
	if (PickupMesh)
	{
		this->SetActorHiddenInGame(false);
		Collider->SetActive(true);

		Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ASTK_PickupBase::Disable()
{
	if (PickupMesh)
	{
		this->SetActorHiddenInGame(true);
		Collider->SetActive(false);

		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void ASTK_PickupBase::BeginPlay()
{
	Super::BeginPlay();
	Origin = Collider->GetRelativeLocation();
}

// Called every frame
void ASTK_PickupBase::Tick(float DeltaTime)
{
	if (!bAnimate)
		return;

	Super::Tick(DeltaTime);

	RotAnimVal += DeltaTime * rotFreq;

	if (bBob) {

		BobAnimVal += DeltaTime * bobFreq;
		if (BobAnimVal > 1)
			BobAnimVal = 0;

		Collider->SetRelativeLocation(Origin + FVector::UpVector * sin(BobAnimVal * 2 * PI) * bobAmp);
	}

	if (bRotate) {

		RotAnimVal += DeltaTime * rotFreq;
		if (RotAnimVal > 1)
			RotAnimVal = 0;

		Collider->SetRelativeRotation(RotDir * RotAnimVal * 360);
	}

	Super::Tick(DeltaTime);

}

EPickupType ASTK_PickupBase::GetPickupType()
{
	return EPickupType::Undefined;
}

// Fill out your copyright notice in the Description page of Project Settings.


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

	Tags.Add("Pickup");
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
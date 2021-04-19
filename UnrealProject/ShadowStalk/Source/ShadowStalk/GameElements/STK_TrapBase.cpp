// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_TrapBase.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASTK_TrapBase::ASTK_TrapBase()
{
	Tags.Add("Trap");

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComp = CreateDefaultSubobject<USceneComponent>("Root");
	SceneRootComp->SetRelativeLocation(FVector(0));
	SetRootComponent(SceneRootComp);

	TrapCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	TrapCollider->SetEnableGravity(false);
	TrapCollider->SetGenerateOverlapEvents(true);
	TrapCollider->SetCollisionProfileName("BlockAll");
	TrapCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TrapCollider->SetRelativeLocation(FVector(0, 0, 40));
	TrapCollider->BodyInstance.bLockRotation;
	TrapCollider->BodyInstance.bLockYTranslation;
	TrapCollider->BodyInstance.bLockXTranslation;
	TrapCollider->SetNotifyRigidBodyCollision(true);
	TrapCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	TrapCollider->SetupAttachment(SceneRootComp);

	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>("Trap Mesh");
	TrapMesh->SetRelativeLocation(FVector(0, 0, 0));
	TrapMesh->SetupAttachment(TrapCollider);
	TrapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	OnActorHit.AddDynamic(this, &ASTK_TrapBase::OnHit);
}

/// <summary>
/// Called when the shade is caught in the trap.
/// Disables the collider and plays a trap sound.
/// </summary>
void ASTK_TrapBase::ActivateTrap()
{
	TrapCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TrapSound, GetActorLocation());
}

void ASTK_TrapBase::DisableTrap()
{
	
}

/// <summary>
/// Called when the trap hits the floor. it disables gravity and enables overlap collision.
/// </summary>
void ASTK_TrapBase::SetTrap()
{
	TrapCollider->SetSimulatePhysics(false);
	TrapCollider->SetEnableGravity(false);
	TrapCollider->SetCollisionProfileName("OverlapAll");
	TrapCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

/// <summary>
/// Called when the trap hits the floor. it disables gravity and enables overlap collision.
/// </summary>
void ASTK_TrapBase::SpawnTrap()
{
	TrapCollider->SetSimulatePhysics(true);
	TrapCollider->SetEnableGravity(true);
}

// Called when the game starts or when spawned
void ASTK_TrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASTK_TrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASTK_TrapBase::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (TrapCollider->IsSimulatingPhysics() == true)
	{
		SetTrap();
	}
}

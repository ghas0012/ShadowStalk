// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_TrapBase.h"
#include "Components/BoxComponent.h"

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
	TrapCollider->SetCollisionProfileName("OverlapAll");
	TrapCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TrapCollider->SetRelativeLocation(FVector(0, 0, 40));
	TrapCollider->SetupAttachment(SceneRootComp);

	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>("Trap Mesh");
	TrapMesh->SetRelativeLocation(FVector(0, 0, 0));
	TrapMesh->SetupAttachment(TrapCollider);
	TrapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASTK_TrapBase::ActivateTrap()
{
	TrapCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASTK_TrapBase::DisableTrap()
{

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


// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity.h"
#include "GameFramework/Controller.h"
#include "../Movement Component/EntityMovementComponent.h"

// Sets default values
AEntity::AEntity()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AEntity::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEntity::Forward(float value)
{

}

void AEntity::Strafe(float value)
{

}

void AEntity::Interact()
{

}

void AEntity::Jump()
{

}

void AEntity::Crawl(bool IsCrawl)
{

}

// Called every frame
void AEntity::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AEntity::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


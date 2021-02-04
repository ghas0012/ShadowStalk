// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"


#include "Entity.generated.h"

UCLASS()
class SHADOWSTALK_API AEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntity();

	class UEntityMovementComponent* m_MovementComp;
	class UCapsuleComponent* m_PlayerCapsule;
	class USphereComponent* m_InteractComp;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Forward(float value);
	virtual void Strafe(float value);
	virtual void Interact();
	virtual void Jump();
	virtual void Crawl(bool IsCrawl);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Entity.generated.h"
#include "../Movement Component/EntityMovementComponent.h"

UCLASS()
class SHADOWSTALK_API AEntity : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntity();

	UEntityMovementComponent* m_MovementComponent;
	UCapsuleComponent* m_CapsuleComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

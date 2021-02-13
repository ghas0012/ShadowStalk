// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "EntityMonster.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API AEntityMonster : public AEntity
{
	GENERATED_BODY()


public:

	AEntityMonster();

	virtual void Forward(float value) override;
	virtual void Strafe(float value) override;
	virtual void Jump() override;
	virtual void Interact() override;

	virtual void MouseLook_Vertical(float value) override;
	virtual void MouseLook_Horizontal(float value) override;


protected:


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

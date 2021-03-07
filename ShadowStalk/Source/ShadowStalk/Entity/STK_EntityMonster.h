// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityMonster.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_EntityMonster : public ASTK_Entity
{
	GENERATED_BODY()


public:

	ASTK_EntityMonster();

	virtual void Interact() override;


protected:


public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

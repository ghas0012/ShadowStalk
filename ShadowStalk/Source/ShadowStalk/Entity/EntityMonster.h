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

protected:

	
};

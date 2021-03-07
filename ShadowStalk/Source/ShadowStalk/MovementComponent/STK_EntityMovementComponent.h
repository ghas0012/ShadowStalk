// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Christian Young, Hamidreza Ghasemi

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "STK_EntityMovementComponent.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_EntityMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class UCapsuleComponent* CapsuleComp;
	FVector VelocityVector = FVector::ZeroVector;
	FVector VelocityAtJump = FVector::ZeroVector;

	float FrictionLerp = 1;
	float Acceleration;
	float WalkSpeed;
	float CurrentSpeed;
	float SprintSpeed;
	float CrawlSpeed;
	float JumpStrength;
	float AirControl = 0;

	bool bIsCrawling = false;

	void Reset();
	void Jump(float jumps); //TODO - FUCKING FIX THIS DUDE HOLY FUCKING SHIT
	void LockInput(bool b);
	void Sprint();
	void Walk();
	void Crawl();
	bool bInputLocked = false;
	bool bIsGrounded = true;

	UFUNCTION(BlueprintCallable)
		bool GetIsGrounded();

	UFUNCTION(BlueprintCallable)
		float GetForwardVelocity();

};

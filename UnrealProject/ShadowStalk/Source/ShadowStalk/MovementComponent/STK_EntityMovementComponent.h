// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi, Jeffrey Armstrong
  Date Modified: 3/12/2021
  Comment/Description: 

	A custom movement component with ground checking, movement handling, jumping and crouching.
	It reads the input acceleration vector and calculates a desired movement per frame.
	It then applies that movement value after resolving all collisions.
	This component partially handles movement replication.

  ChangeLog:
  C 3/12/2021: Class init. Added movement, ground checking and collision resolution.
  J 3/12/2021: Added crouch functionality.
  H 3/12/2021: Added a smooth height transition for crouching.
  H 3/16/2021: Added a class description and summaries to relevant methods.
*/

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

	UPROPERTY(Replicated)
	class UCapsuleComponent* CapsuleComp;

	FVector VelocityVector = FVector::ZeroVector;
	FVector VelocityAtJump = FVector::ZeroVector;

	float FrictionLerp = 1;
	float AirControl = 1;
	float Acceleration;

	UPROPERTY(Replicated)
	float CurrentSpeed;

	float JumpStrength;
	float SprintSpeed;
	float CrawlSpeed;
	float WalkSpeed;

	float CrawlTransitionSpeed = 5.f;
	float CrawlTransitionPercentage = 0.f;
	float CrawlTransitionInitHalfHeight;

	float CapsuleStandingHalfHeight;
	float CapsuleCrawlHalfHeight;

	bool bIsCrawling = false;

	void HandleCrawlTransition(float DeltaTime);

	void Reset();

	void Jump(float jumpStrength); //TODO - FIX THIS DUDE HOLY CRAP

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

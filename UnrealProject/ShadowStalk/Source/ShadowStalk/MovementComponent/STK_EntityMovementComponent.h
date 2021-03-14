// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi, Jeffrey Armstrong
  Date Modified: 3/12/2021
  Comment/Description: *Description of Class*
  ChangeLog:
  C 3/12/2021: *Description of Change*
  H 3/12/2021: *Description of Change*
  J 3/12/2021: *Description of Change*
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

	class UCapsuleComponent* CapsuleComp;

	FVector VelocityVector = FVector::ZeroVector;
	FVector VelocityAtJump = FVector::ZeroVector;

	float FrictionLerp = 1;
	float AirControl = 1;
	float Acceleration;

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

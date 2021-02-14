// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "EntityMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API UEntityMovementComponent : public UPawnMovementComponent
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
	bool bIsGrounded = false;

};

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
	float MaxWalkSpeed;
	float CrawlWalkSpeed;
	float AirControl = 0;

	void Reset();
	void Jump(float JumpStrength);
	void LockInput(bool b);
	void Crawl();
	bool bInputLocked = false;
	bool bIsGrounded = false;

};

// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: 

	A wrapper around the APawn class that allows for shared functionality such as movement behavior and interaction between the Shade and Monster classes.
	The Entity class is an abstract class, which means it will never appear alone and is simply meant as a template class for all of the game’s entities.
		
  ChangeLog:
  C 3/12/2021: Created the base entity class, added movement variables, implemented skeletal mesh.
  H 3/12/2021: Added entity type getters, input locking, camera and position override, and cleaned up the Tick function.
  H 3/16/2021: Reformatted and cleaned up the methods and their order. Added a class description and summaries for pertinent methods.
*/

#pragma once

#include "CoreMinimal.h"
#include "ShadowStalk/ShadowStalk.h"
#include "GameFramework/Character.h"
#include "STK_Entity.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_Entity : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASTK_Entity();

	//TODO - make Editanywhere


	UPROPERTY(Replicated)
	class USTK_EntityMovementComponent* m_MovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class UCapsuleComponent* m_PlayerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* m_CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class USkeletalMeshComponent* m_MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_MouseLook_X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_MouseLook_Y;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_MouseLook_VerticalLookLimitAngle = 85.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_JumpStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_FrictionLerp = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_CrawlSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_CapsuleRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_CapsuleHalfHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float m_CrawlCapsuleHalfHeight = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	bool bCameraOverride = false;

	//Respawn won't have a function, however if we want to add this, the framework is here.
	UFUNCTION()
	virtual void Respawn() {}

	// position override variables
	bool bPositionOverride = false;
	float PositionOverridePercent;
	FVector PositionOverrideTarget;
	FVector PositionOverrideOrigin;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Restart() override;

	virtual void PostInitializeComponents() override;

	virtual void HandleCamera(float DeltaTime);

	UPROPERTY(Replicated)
	FVector MouseLookVector = FVector::ZeroVector;

	FVector ForwardAccelerationVector;

	FVector RightAccelerationVector;
	FVector VelocityVector;

	bool bIsGrounded;
	bool bCanJump;

	float ForwardInput = 0.0f;

	FVector CameraOverrideTarget;
	float CameraOverrideSpeed = 10.f; 
	float PositionOverrideSpeed = 30.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		class USoundBase* FootstepsSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Custom", meta = (AllowPrivateAccess = "true"))
		float FootstepFrequency;

		float FootstepTimer;

	uint8 InputLockFlags = 0;
	
public:

	UFUNCTION(BlueprintCallable)
	virtual EEntityType GetEntityType();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandlePosition(float DeltaTime);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HandlePosition(float DeltaTime);

	void HandleFootstepSounds(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetInputLock(EInputLockFlags flag, bool lock);
	void SetInputLock(uint8 flag, bool lock);

    // virtual UPawnMovementComponent* GetMovementComponent();

	virtual void Forward(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Forward(float value);

	virtual void Strafe(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Strafe(float value);

	void LockCameraLookat(FVector Offset);
	void UnlockCameraLookat();
	
	void ForceMoveToPoint(FVector target);

	virtual void Interact();

	virtual void Jump();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Jump();

	virtual void Sprint(bool IsSprint);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Sprint(bool IsSprint);


	virtual void Crawl(bool IsCrawl);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Crawl(bool IsCrawl);

	virtual void UnhideMouse();
	virtual void HideMouse();

	virtual void MouseLook_Vertical(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MouseLook_Vertical(float value);

	virtual void MouseLook_Horizontal(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MouseLook_Horizontal(float value);

	bool GetIsGrounded();
};

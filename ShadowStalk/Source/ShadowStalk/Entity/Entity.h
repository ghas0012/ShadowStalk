// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"


#include "Entity.generated.h"

UCLASS()
class SHADOWSTALK_API AEntity : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEntity();

	//TODO - make Editanywhere

	UPROPERTY()
	class UEntityMovementComponent* m_MovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class UCapsuleComponent* m_PlayerCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	class USphereComponent* m_InteractComp;

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
		float m_MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float m_AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float m_CapsuleRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float m_CapsuleHalfHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		float m_InteractRadius = 50.f;


	//Respawn won't have a function, however if we want to add this, the framework is here.
	UFUNCTION()
		virtual void Respawn() {}


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleCamera();

	FVector MouseLookVector = FVector::ZeroVector;
	FVector ForwardAccelerationVector;
	FVector RightAccelerationVector;
	FVector VelocityVector;


	bool bIsGrounded;
	bool bCanJump;

	float ForwardInput = 0.0f;



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//	virtual UPawnMovementComponent* GetMovementComponent();

	virtual void Forward(float value);
	virtual void Strafe(float value);
	virtual void Interact();
	virtual void Jump();
	virtual void Crawl(bool IsCrawl);

	virtual void UnhideMouse();
	virtual void HideMouse();

	virtual void MouseLook_Vertical(float value);
	virtual void MouseLook_Horizontal(float value);

	bool GetIsGrounded();


};

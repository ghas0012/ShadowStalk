// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi, Arianne Fennell
  Date Modified: 3/12/2021
  Comment/Description:

    A wrapper around the APawn class that allows for shared functionality such as movement behavior and interaction between the Shade and Monster classes.
    The Entity class is an abstract class, which means it will never appear alone and is simply meant as a template class for all of the game’s entities.

  ChangeLog:
  C 3/12/2021: Created the base entity class, added movement variables, implemented skeletal mesh.
  H 3/12/2021: Added entity type getters, input locking, camera and position override, and cleaned up the Tick function.
  H 3/16/2021: Reformatted and cleaned up the methods and their order. Added a class description and summaries for pertinent methods.
  H 3/23/2021: Added FSTK_EntityData, Moved movement data into that struct.
  A 3/23/2021: Added function that sets up the Pause Menu.
*/

#pragma once

#include "CoreMinimal.h"
#include "ShadowStalk/ShadowStalk.h"
#include "GameFramework/Character.h"
#include "STK_EntityData.h"
#include "STK_Entity.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_Entity : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASTK_Entity();

    UPROPERTY()
    class USTK_EntityMovementComponent* m_MovementComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    class UCapsuleComponent* m_PlayerCapsule;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    class UCameraComponent* m_CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    class USkeletalMeshComponent* m_MeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    bool bCameraOverride = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
    FSTK_EntityData m_MovementData;

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

    virtual void HandleCamera(float DeltaTime);

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

    void HandleFootstepSounds(float DeltaTime);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void SetInputLock(EInputLockFlags flag, bool lock);
    void SetInputLock(uint8 flag, bool lock);

    virtual void Forward(float value);
    virtual void Strafe(float value);

    void LockCameraLookat(FVector Offset);
    void UnlockCameraLookat();

    void ForceMoveToPoint(FVector target);

    virtual void Interact();
    virtual void Jump();
    virtual void Sprint(bool IsSprint);
    virtual void Crawl(bool IsCrawl);

    virtual void MouseLook_Vertical(float value);
    virtual void MouseLook_Horizontal(float value);

    virtual void SetupPauseMenu();

    bool GetIsGrounded();
};

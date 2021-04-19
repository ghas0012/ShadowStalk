// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Christian Young, Hamidreza Ghasemi, Arianne Fennell, Jeffrey Armstrong
  Date Modified: 3/12/2021
  Comment/Description:

    A wrapper around the APawn class that allows for shared functionality such as movement behavior and interaction between the Shade and Monster classes.
    The Entity class is an abstract class, which means it will never appear alone and is simply meant as a template class for all of the game's entities.

  ChangeLog:
  C 3/12/2021: Created the base entity class, added movement variables, implemented skeletal mesh.
  H 3/12/2021: Added entity type getters, input locking, camera and position override, and cleaned up the Tick function.
  H 3/16/2021: Reformatted and cleaned up the methods and their order. Added a class description and summaries for pertinent methods.
  C 3/18/2021: Added Networking code to Entity.h and Entity.cpp
  H 3/23/2021: Added FSTK_EntityData, Moved movement data into that struct.
  A 3/23/2021: Added function that sets up the Pause Menu.
  J 3/31/2021: Added another footsteps sound and additional variables and code for Inventory Component.
  H 4/12/2021: Swapped to character. includes changes to lookat, moveto, execution, animation blueprint, attack, controllers, gamemode, gamestate.
  J 4/14/2021: Adjustments for Pickup variables.
  H 4/16/2021: Networked sprint. Added sound attenuation to footsteps.
*/

#pragma once

#include "CoreMinimal.h"
#include "ShadowStalk/ShadowStalk.h"
#include "GameFramework/Character.h"
#include "STK_EntityData.h"
#include "STK_EntityCharacter.generated.h"

UCLASS()
class SHADOWSTALK_API ASTK_EntityCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASTK_EntityCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity|Base")
    class UCameraComponent* m_CameraComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity|Base")
    class USkeletalMeshComponent* m_TPMeshComp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Entity|Base")
    FSTK_EntityData m_MovementData;

    TSubclassOf<class UUserWidget> PauseMenuClass;
    class USTK_UWPauseMenu* UWPauseMenu;

    UPROPERTY(Replicated)
    uint8 InputLockFlags = 0;

    UPROPERTY(Replicated, BlueprintReadOnly)
    FRotator CurrentControllerFacing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USoundAttenuation* FootstepAttenuation;

protected:

    // position override variables
    bool bPositionOverride = false;
    float PositionOverridePercent = 0;
    FVector PositionOverrideTarget;
    FVector PositionOverrideOrigin;
    float PositionOverrideSpeed = 3.f;

    // camera override variables
    bool bCameraOverride = false;
    float CameraOverridePercent = 0;
    class USceneComponent* CameraOverrideTarget;
    FRotator CameraOverrideOrigin;
    float CameraOverrideSpeed = 4.f;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    class ASTK_PickupBase* CurrentItem;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity|Inventory", meta = (AllowPrivateAccess = "true"))
    class USTK_InventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
    class USoundBase* FootstepsSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
    class USoundBase* FootstepsSound2;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Entity|Audio", meta = (AllowPrivateAccess = "true"))
    float FootstepFrequency;

    float FootstepTimer;

    bool PlayFootstep1 = true;
    bool PlayFootstep2 = false;

public:

    /// Input functions
#pragma region

    virtual void Forward(float value);

    virtual void Strafe(float value);

    virtual void Interact();

    virtual void Jump();

    virtual void Sprint(bool IsSprint);

    virtual void Crawl(bool IsCrawl);

    virtual void MouseLook_Vertical(float value);

    virtual void MouseLook_Horizontal(float value);

    virtual void PauseMenu();

    virtual void NextItem();

    virtual void PrevItem();

    virtual void UseItem();

#pragma endregion

    void LockCameraLookat(USceneComponent* SceneComp);
    void UnlockCameraLookat();

    UFUNCTION(Client, Reliable)
    void Client_LockCameraLookat(USceneComponent* SceneComp);

    void ForceMoveToPoint(FVector target);

    UFUNCTION(BlueprintCallable)
    virtual EEntityType GetEntityType() { return EEntityType::Undefined; };

    void TurnAtRate(float Rate);
    void LookUpAtRate(float Rate);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    void HandlePositionOverride(float DeltaTime);
    void HandleCamera(float DeltaTime);

    void HandleFootstepSounds(float DeltaTime);

    void SetInputLock(EInputLockFlags flag, bool lock);
    void SetInputLock(uint8 flag, bool lock);

    UFUNCTION(Server, Reliable)
    void Server_SetInputLock(uint8 flag, bool lock);

    UFUNCTION(Server, Reliable)
    void Server_Sprint(bool IsSprint);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

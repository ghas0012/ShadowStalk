// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

/*
  Author: Hamidreza Ghasemi
  Date Modified: 3/12/2021
  Comment/Description: 
  
    The main driver of the Shade players’ character.
    It reacts to user input, picks up pickup instances, interacts with the Inventory Module, controls the Shade’s eyes,
    and accepts requests to take damage or be executed.
  
  ChangeLog:
  H 3/12/2021: Class init. Added eye component and example use case.
  H 3/12/2021: Added States, health and relevant methods for recieving attacks and being executed.
  H 3/16/2021: Added a class description and summaries to relevant methods.
  C 3/19/2021: Added networking code.
  H 3/23/2021: Modified the attack logic so: 1. The shade jumps when hit. 2. The shade only plays knockback anim when downed. 3. The shade ignores pawn collisions when downed, and safely stops ignoring them after recovering.
  H 3/23/2021: Moved movement data into its own struct.
  J 3/28/2021: Added compatibility with the Trap class. The Shade will get stuck for brief moment when it collides with a trap. RecoverFromTrap() handles the Shade being freed from the trap.
  H 3/31/2021: Added Blinking input. Networked it. Added first person Blinking. Added vertical head movement and smooth head rotation. Modified first person light. Added first person mesh separate from 3rd person mesh.
  J 3/31/2021: Added variables and code for Inventory Component.
  H 4/12/2021: Swapped to character. includes changes to lookat, moveto, execution, animation blueprint, attack, controllers, gamemode, gamestate.
  J 4/12/2021: Fixed Hit Sound.
  J 4/14/2021: Added Item Types for Pickup.

*/

#pragma once

#include "CoreMinimal.h"
#include "STK_EntityCharacter.h"
#include "STK_EntityCharacterShade.generated.h"

UENUM(BlueprintType)
enum class ECharacterShadeState : uint8 
{
    Execution   UMETA(DisplayName = "Execution"),
    Default     UMETA(DisplayName = "Default"),
    Downed      UMETA(DisplayName = "Downed"),
    Hurt        UMETA(DisplayName = "Hurt"),
    KnockedBack UMETA(DisplayName = "Hit"),
    Dead	    UMETA(DisplayName = "Dead"),
    Stuck       UMETA(DisplayName = "Stuck")
};

UCLASS()
class SHADOWSTALK_API ASTK_EntityCharacterShade : public ASTK_EntityCharacter
{
    GENERATED_BODY()

public:

    virtual EEntityType GetEntityType() override;

    ASTK_EntityCharacterShade();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Eyes")
        class USTK_EyeComponent* m_pEyes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Eyes")
        class USkeletalMeshComponent* m_pEyeSocket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Eyes")
        class URectLightComponent* m_pLSpotlight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Eyes")
        class URectLightComponent* m_pRSpotlight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Eyes")
        class URectLightComponent* m_pFPSpotlight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Eyes")
        float BlinkSpeed = 1;

    //Sound

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shade|Audio", meta = (AllowPrivateAccess = "true"))
        class USoundBase* ShadeTeleportSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shade|Audio", meta = (AllowPrivateAccess = "true"))
        class USoundBase* ShadeDownGroundHit;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shade|Audio", meta = (AllowPrivateAccess = "true"))
        class USoundBase* ShadeItemPickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shade|Audio", meta = (AllowPrivateAccess = "true"))
        class USoundBase* HitSound;


protected:

    float BlinkPercentage;

    UPROPERTY(Replicated)
    float BlinkTarget;

    // Called when the game starts
    virtual void BeginPlay() override;

    void SetupEyes();

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(Replicated)
    ECharacterShadeState CurrentState = ECharacterShadeState::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Health")
        int Health = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Health")
        float DownedRecoveryTime = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Health")
        float KnockbackRecoveryDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Health")
        float KnockbackStandupDuration = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shade|Health")
        float StuckRecoveryTime = 6.0f;

    void RecoverFromDowned();

    void RecoverFromTrap();

    void SafeActivatePawnCollision();

    FTimerHandle DownedRecoveryHandle;
    FTimerHandle SafeActivatePawnCollisionHandle;
    bool OverlappingAnotherEntity = false;

    FTimerHandle DelayedStateChangeHandle;
    ECharacterShadeState DelayedTargetState;

    FTimerHandle StuckRecoveryHandle;

    void DelayedStateChange();

    // UFUNCTION(Client, Reliable)
    // void Client_Spectate();

    float InitBrightness;

public:

    void StartExecution(class ASTK_EntityCharacterMonster* Executioner);

    UFUNCTION(Server, Reliable)
    void Server_StartExecution(class ASTK_EntityCharacterMonster* Executioner);

    void ApplyDamage(unsigned char damage, FVector knockback);

    UFUNCTION(BlueprintCallable)
        int GetHealth();

    //Might have to Network later? 
    UFUNCTION(BlueprintCallable)
        void SetHealth(int targetHealth);
 
    UFUNCTION(BlueprintCallable)
        ECharacterShadeState GetShadeState();

    UFUNCTION(BlueprintCallable)
        void SetShadeState(ECharacterShadeState state);
 
    UFUNCTION(Server, Reliable)
        void Server_SetShadeState(ECharacterShadeState state);
    
    void Blink(bool blink);

    UFUNCTION(Server, Reliable)
    void Server_Blink(bool blink);

    void HandleBlinkInput(float DeltaTime);

    // UFUNCTION(Server, Reliable)
    // void Server_Spectate();
    
    //void Server_HandleBlinkInput(float DeltaTime);

    virtual void Interact() override;

    virtual void Tick(float DeltaTime) override;

};

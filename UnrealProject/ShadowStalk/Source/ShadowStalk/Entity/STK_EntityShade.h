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
*/

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityShade.generated.h"

UENUM(BlueprintType)
enum class EShadeState : uint8 
{
    Execution   UMETA(DisplayName = "Execution"),
    Default     UMETA(DisplayName = "Default"),
    Downed      UMETA(DisplayName = "Downed"),
    Hurt        UMETA(DisplayName = "Hurt"),
    KnockedBack UMETA(DisplayName = "Hit"),
    Dead	    UMETA(DisplayName = "Dead")
};

UCLASS()
class SHADOWSTALK_API ASTK_EntityShade : public ASTK_Entity
{
    GENERATED_BODY()

public:

    virtual EEntityType GetEntityType() override;

    ASTK_EntityShade();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
        class USTK_EyeComponent* m_pEyes;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
        class URectLightComponent* m_pLSpotlight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
        class URectLightComponent* m_pRSpotlight;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    void SetupEyes();

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    EShadeState CurrentState = EShadeState::Default;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    int Health = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float DownedRecoveryTime = 10.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float KnockbackRecoveryDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
        float KnockbackStandupDuration = 0.3f;


    void RecoverFromDowned();

    FTimerHandle DownedRecoveryHandle;
    FTimerHandle DelayedStateChangeHandle;
    EShadeState DelayedTargetState;

    void DelayedStateChange();

public:


    void StartExecution(class ASTK_EntityMonster* Executioner);

    UFUNCTION(Server, Reliable)
    void Server_StartExecution(class ASTK_EntityMonster* Executioner);

 
    void ApplyDamage(unsigned char damage, FVector knockback);


    UFUNCTION(BlueprintCallable)
        int GetHealth();

    //Might have to Network later? 
    UFUNCTION(BlueprintCallable)
        void SetHealth(int targetHealth);
 
    UFUNCTION(BlueprintCallable)
        EShadeState GetShadeState();

    UFUNCTION(BlueprintCallable)
        void SetShadeState(EShadeState state);
 

    virtual void Interact() override;

    virtual void Tick(float DeltaTime) override;

};

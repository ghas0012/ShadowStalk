// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.
// - Added States
// - Added Health and relevant funcs

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityShade.generated.h"

UENUM(BlueprintType)
enum class EShadeState : uint8 {
    Default  UMETA(DisplayName = "Default"),
    Dead	 UMETA(DisplayName = "Dead"),
    Hurt     UMETA(DisplayName = "Hurt")
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

    int Health = 2;

public:

    void GetHurt(unsigned char damage);

    UFUNCTION(BlueprintCallable)
        int GetHealth();

    UFUNCTION(BlueprintCallable)
        void SetHealth(int health);

    UFUNCTION(BlueprintCallable)
        EShadeState GetShadeState();

    UFUNCTION(BlueprintCallable)
        void SetShadeState(EShadeState state);

    virtual void Tick(float DeltaTime) override;

};

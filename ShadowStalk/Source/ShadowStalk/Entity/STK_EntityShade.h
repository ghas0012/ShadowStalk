// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Hamidreza Ghasemi

// Changelog:
// - Class init.

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityShade.generated.h"

UENUM(BlueprintType)
enum class E_ShadeState : uint8 {
    Default  UMETA(DisplayName = "Default"),
    Dead	 UMETA(DisplayName = "Dead"),
    Hurt     UMETA(DisplayName = "Hurt")
};

UCLASS()
class SHADOWSTALK_API ASTK_EntityShade : public ASTK_Entity
{
    GENERATED_BODY()


public:

    ASTK_EntityShade();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
        class USTK_EyeComponent* m_pEyes;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
        class URectLightComponent* m_pLSpotlight;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
        class URectLightComponent* m_pRSpotlight;

    UFUNCTION(BlueprintCallable)
        E_ShadeState GetShadeState();

    UFUNCTION(BlueprintCallable)
        void SetShadeState(E_ShadeState state);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    void SetupEyes();

    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    E_ShadeState CurrentState = E_ShadeState::Default;

public:

    virtual void Tick(float DeltaTime) override;

};

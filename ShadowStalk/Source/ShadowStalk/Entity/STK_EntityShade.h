// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STK_Entity.h"
#include "STK_EntityShade.generated.h"

/**
 *
 */
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


protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    void SetupEyes();

    UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

    virtual void Tick(float DeltaTime) override;

};

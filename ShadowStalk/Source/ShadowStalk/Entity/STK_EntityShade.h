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

    // TODO: add eye lights thru code
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    //    class ASpotlight* m_pLSpotlight;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    //    class ASpotlight* m_pRSpotlight;

    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
};

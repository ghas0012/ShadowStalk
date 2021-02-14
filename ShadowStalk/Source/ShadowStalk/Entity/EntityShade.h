// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entity.h"
#include "EntityShade.generated.h"

/**
 *
 */
UCLASS()
class SHADOWSTALK_API AEntityShade : public AEntity
{
    GENERATED_BODY()

    
public:

    AEntityShade();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Eyes")
    class UEyeComponent* m_pEyes;

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

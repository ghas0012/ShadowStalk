// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "STK_MainMenuLevel.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWSTALK_API ASTK_MainMenuLevel : public ALevelScriptActor
{
    GENERATED_BODY()

protected:
    //Setup level specific code
    virtual void BeginPlay() override;
};

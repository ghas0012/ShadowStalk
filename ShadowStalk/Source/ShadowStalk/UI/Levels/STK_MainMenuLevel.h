// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.
// Author: Arianne Fennell

// Changelog:
// - Class init.

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

    UFUNCTION()
    void LoadMainMenu();
};

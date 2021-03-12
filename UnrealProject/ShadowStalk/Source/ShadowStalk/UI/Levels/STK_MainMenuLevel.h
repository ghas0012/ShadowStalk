// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/12/2021
  Comment/Description: LevelScriptActor associated with the Main Menu Level.
  ChangeLog:
  3/12/2021: Implemented base functions to work with UWMainMenu.
  */

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "STK_MainMenuLevel.generated.h"

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

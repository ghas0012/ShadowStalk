// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

 /*
  Author: Arianne Fennell
  Date Modified: 3/23/2021
  Comment/Description: Base Game Instance for the game.
  ChangeLog:
  A 3/12/2021: Implemented base functions to work with UWMainMenu.
  A 3/13/2021: Added SetupCreditWidget() to work with UWCreditsPanel.
  A 3/23/2021: Moved Options and Credits setup to UWMainMenu.
  */

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STK_GameInstance.generated.h"

UCLASS()
class SHADOWSTALK_API USTK_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    USTK_GameInstance(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void SetupMainMenuWidget();

    UFUNCTION(BlueprintCallable)
    void LoadGameLevel();

private:
    TSubclassOf<class UUserWidget> MainMenuClass;
    class USTK_UWMainMenu* UWMainMenu;
};

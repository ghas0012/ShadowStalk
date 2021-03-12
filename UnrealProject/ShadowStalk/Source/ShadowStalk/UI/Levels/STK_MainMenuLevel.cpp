// Copyright (C) Particle Interactive Ltd. 2021. All Rights Reserved.

#include "STK_MainMenuLevel.h"

#include "ShadowStalk/GameInstance/STK_GameInstance.h"

void ASTK_MainMenuLevel::BeginPlay()
{
    Super::BeginPlay();

    LoadMainMenu();
}

/// <summary>
/// Loads the Main Menu widget from the Game Instance.
/// </summary>
void ASTK_MainMenuLevel::LoadMainMenu()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());

    if (GameInstance == nullptr) return;

    GameInstance->LoadMainMenuWidget();
}

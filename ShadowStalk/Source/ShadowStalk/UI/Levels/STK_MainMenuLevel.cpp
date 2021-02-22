// Fill out your copyright notice in the Description page of Project Settings.


#include "STK_MainMenuLevel.h"

#include "ShadowStalk/GameInstance/STK_GameInstance.h"

void ASTK_MainMenuLevel::BeginPlay()
{
    Super::BeginPlay();

    LoadMainMenu();
}

void ASTK_MainMenuLevel::LoadMainMenu()
{
    auto GameInstance = Cast<USTK_GameInstance>(GetGameInstance());

    if (GameInstance == nullptr) return;

    GameInstance->LoadMainMenuWidget();
}
